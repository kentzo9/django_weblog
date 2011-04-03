from django.utils.feedgenerator import Atom1Feed
from django.contrib.sites.models import Site
from django.contrib.syndication.feeds import Feed
from coltrane.models import Entry

current_site = Site.objects.get_current()

class LatestEntriesFeed(Feed):
    author_name = "KentZo"
    copyright = "http://%s/about/copyright/" % current_site.domain
    description = "Latest entries posted to %s" % current_site.name
    feed_type = Atom1Feed
    item_copyright = "http://%s/about/copyright/" % current_site.domain
    item_author_name = "Bob Smith"
    item_author_link = "http://%s/" % current_site.domain
    link = "/feeds/entries/"
    title = "%s: Latest entries" % current_site.name

    def items(self):
       return Entry.live.all()[:10]
    def item_pubdate(self,item):
       return item.pub_date
    def item_guid(self, item):
       return "tag:%s,%s:%s" % (current_site.domain,
                                 item.pub_date.strftime('%Y-%m-%d'),
                                 item.get_absolute_url())
    def item_categories(self, item):
       return [c.title for c in item.categories.all()]
    def item_link(self,item):
       print type(item)
       print item.__class__.__name__
       print isinstance(item,Entry)
       print item.title
       print item.pub_date
       return item.get_absolute_url()
