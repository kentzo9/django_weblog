from django.db import models
import datetime
from django.contrib.auth.models import User
from tagging.fields import TagField
from markdown import markdown
from django.conf import settings

class Category(models.Model):
   title = models.CharField(max_length=250, help_text='Maximum 250 characters.')
   slug = models.SlugField(unique=True, help_text='Suggested value automatically generated from title. Must be unique')
   description = models.TextField()

   class Meta:
      ordering = ['title']
      verbose_name_plural = "Categories"

   def __unicode__(self):
      return self.title
   
##   def get_absolute_url(self):
##      return "categories/%s/" % self.slug
   def get_absolute_url(self):
     return ('coltrane_category_detail',(),{'slug':self.slug})
   get_absolute_url = models.permalink(get_absolute_url)
   
   def live_entry_set(self):
      from coltrane.models import Entry
      return self.entry_set.filter(status=Entry.LIVE_STATUS)

#subclass of subclass of django.db.models.Manager, meant to override the get_query_set
class LiveEntryManager(models.Manager):
   def get_query_set(self):
      return super(LiveEntryManager, self).get_query_set().filter(status=self.model.LIVE_STATUS)

class Entry(models.Model):
   LIVE_STATUS = 1
   DRAFT_STATUS = 2
   HIDDEN_STATUS = 3
   STATUS_CHOICES = (
      (LIVE_STATUS, 'Live'),
      (DRAFT_STATUS, 'Draft'),
      (HIDDEN_STATUS, 'Hidden'),
   )
   title = models.CharField(max_length = 250)
   excerpt = models.TextField(blank=True)
   body = models.TextField()
   pub_date = models.DateTimeField(default=datetime.datetime.now) #only pass in the function name without the (), not the return value from calling a function, if the () is keyed in, this function will be called onced only
   slug = models.SlugField(unique_for_date='pub_date')
   author = models.ForeignKey(User)
   enable_comments = models.BooleanField(default=True)
   status = models.IntegerField(choices=STATUS_CHOICES, default=LIVE_STATUS)
   categories = models.ManyToManyField(Category) #ManyToMany is used to relate this entry to more than one Category object, this will create a new join table containing entry as entry_id field and category as  category_id which is references to class category. 
   excerpt_html = models.TextField(editable=False, blank=True)
   body_html = models.TextField(editable=False, blank=True)
   tags = TagField()
   ##defining own Manager
   ## the seuquence here is important, if placed before objects,
   ## we can see the entries that are live at the website
   ## however the entry does not appear in the Admin interface.
   ##live = LiveEntryManager()
   objects = models.Manager()
   live = LiveEntryManager()
  

   class Meta:
      ordering = ['pub_date']
      verbose_name_plural = "Entries"

   def __unicode__(self):
      return self.title
   
   def get_absolute_url(self):
     #return "%s/%s/" % (self.pub_date.strftime("%Y/%b/%d").lower(), self.slug)
     return ('coltrane_entry_detail',(),{'year':self.pub_date.strftime("%Y"),\
                                         'month':self.pub_date.strftime("%b").lower(),\
                                         'day':self.pub_date.strftime("%d"),\
                                         'slug':self.slug})
   get_absolute_url = models.permalink(get_absolute_url)
   
   def save(self, force_insert=False, force_update=False):
      self.body_html = markdown(self.body)
      print ("body_html=%s" %  self.body_html)
      if self.excerpt:
         self.excerpt_html = markdown(self.excerpt)
      super(Entry, self).save(force_insert, force_update)

class Link(models.Model):
   #Metadata
   enable_comments = models.BooleanField(default=True)
   post_elsewhere = models.BooleanField('Post to Delicious', default=True, help_text='If checked,this link will be posted both to your weblog and to your del.icio.us account.')
   title = models.CharField(max_length=250)
   posted_by = models.ForeignKey(User)
   pub_date = models.DateTimeField(default=datetime.datetime.now)
   slug = models.SlugField(unique_for_date='pub_date')
   #the actual link bits
   description = models.TextField(blank=True)
   description_html = models.TextField(editable=False, blank=True)
   via_name = models.CharField('Via', max_length=250, blank=True, help_text='The name of the person whose site you spotted the link on. Optional.')
   via_url = models.CharField('Via', max_length=250, blank=True, help_text='The URL of the site where you spotted the link. Optional.')
   tags = TagField()
   url = models.URLField(unique=True)

   class Meta:
      ordering = ['-pub_date']
   
   def __unicode__(self):
      return self.title

   def save(self):
      if self.description:
         self.description_html = markdown(self.description)
      if not self.id and self.post_elsewhere:
         import pydelicious
         from django.utils.encoding import smart_str
         pydelicious.add(settings.DELICIOUS_USER, settings.DELICIOUS_PASSWORD,smart_str(self.url), smart_str(self.title), smart_str(self.tags))
      super(Link, self).save()

   def get_absolute_url(self):
     return ('coltrane_link_detail',(),{'year':self.pub_date.strftime("%Y"),\
                                        'month':self.pub_date.strftime("%b").lower(),\
                                        'day':self.pub_date.strftime("%d"),\
                                        'slug':self.slug})
   get_absolute_url = models.permalink(get_absolute_url)

from django.contrib.comments.models import Comment
from django.db.models import signals
from django.contrib.comments.signals import comment_will_be_posted
from django.contrib.sites.models import Site
from akismet import Akismet

def moderate_comment(sender,instance,**kwargs):
    if not instance.id:
        entry = instance.content_object
        print "new comment, check how old is the entry"
        delta = datetime.datetime.now() - entry.pub_date
        if delta.days > 30:
            instance.is_public = False

def moderate_comment2(sender, comment, request, **kwargs):
   print "AGENT: "+request.META['HTTP_USER_AGENT']
   print "REFER: "+request.META['HTTP_REFERER']
   if not comment.id:
      entry = comment.content_object
      delta = datetime.datetime.now() - entry.pub_date
      if delta.days > 30:
         comment.is_public = False
      else:
         akismet_api = Akismet(key=settings.AKISMET_API_KEY,
                               blog_url="http:/%s/"%Site.objects.get_current().domain)
         if akismet_api.verify_key():
            akismet_data = { 'comment_type': 'comment',
                             'referrer': request.META['HTTP_REFERER'],
                             'user_ip': comment.ip_address,
                             'user-agent': request.META['HTTP_USER_AGENT'] }
            if akismet_api.comment_check(smart_str(comment.comment),akismet_data,build_data=True):
               comment.is_public = False

##in models.signals, pre_save is a instance of Signals
## pre_save was called with pre_save.send() in the base models
## the following is the  function attached to that signal.
## so when the signal is sent out by pre_save, this function will receive it
## and get executed
signals.pre_save.connect(moderate_comment, sender=Comment)
comment_will_be_posted.connect(moderate_comment2,sender=Comment)














































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































