from django.conf.urls.defaults import *
from coltrane.models import Entry

entry_info_dict = {
   'queryset' : Entry.objects.all(),
   'date_field': 'pub_date',
   'template_object_name' : 'entry',
}

entry_info_dict1 = {
   'queryset' : Entry.objects.all(),
   'date_field': 'pub_date',
   'template_name' : 'coltrane/entry_index.html', #this will replace the default template, which is entry_archive.html
   'template_object_name' : 'entry_list', #OVER WRITE THE default name latest
}

urlpatterns = patterns('django.views.generic.date_based',
    #(r'^weblog/$', 'coltrane.views.entries_index'),
    (r'^$', 'archive_index',entry_info_dict1),
    #(r'^weblog/weblog/(?P<year>\d{4})/(?P<month>\w{3})/(?P<day>\d{2})/(?P<slug>[-\w]+)/$', 'coltrane.views.entry_detail'),
    # the following is using the generic view to repeat the above task
    (r'^(?P<year>\d{4})/(?P<month>\w{3})/(?P<day>\d{2})/(?P<slug>[-\w]+)/$', 'object_detail', entry_info_dict,'coltrane_entry_detail'), #using entry_detail.html
)