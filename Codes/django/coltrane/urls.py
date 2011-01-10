from django.conf.urls.defaults import *
from coltrane.models import Category, Entry, Link

entry_info_dict = {
   'queryset' : Entry.objects.all(),
   'date_field': 'pub_date',
}
entry_info_dict_index = {
   'queryset' : Entry.objects.all(),
   'date_field': 'pub_date',
   'template_name' : 'coltrane/entry_index.html', #this will replace the default template, which is entry_archive.html
   'template_object_name' : 'entry_list', #OVER WRITE THE default name latest
}
entry_info_dict_year = {
   'queryset' : Entry.objects.all(),
   'date_field': 'pub_date',
   'make_object_list' : True,
}

link_info_dict = {
   'queryset' : Link.objects.all(),
   'date_field': 'pub_date',
}
link_info_dict_year = {
   'queryset' : Link.objects.all(),
   'date_field': 'pub_date',
   'make_object_list' : True,
}

urlpatterns = patterns('django.views.generic.date_based',
    #(r'^weblog/$', 'coltrane.views.entries_index'),
    (r'^$', 'archive_index',entry_info_dict_index,'coltrane_entry_archive_index'),
    (r'^(?P<year>\d{4})/$', 'archive_year', entry_info_dict_year,'coltrane_entry_archive_year'),
    (r'^(?P<year>\d{4})/(?P<month>\w{3})/$', 'archive_month', entry_info_dict,'coltrane_entry_archive_month'),
    (r'^(?P<year>\d{4})/(?P<month>\w{3})/(?P<day>\d{2})/$', 'archive_day', entry_info_dict,'coltrane_entry_archive_day'),
    #(r'^weblog/weblog/(?P<year>\d{4})/(?P<month>\w{3})/(?P<day>\d{2})/(?P<slug>[-\w]+)/$', 'coltrane.views.entry_detail'),
    # the following is using the generic view to repeat the above task
    (r'^(?P<year>\d{4})/(?P<month>\w{3})/(?P<day>\d{2})/(?P<slug>[-\w]+)/$', 'object_detail', entry_info_dict,'coltrane_entry_detail'), #using entry_detail.html   
    ## for link 
    (r'^links/$', 'archive_index',link_info_dict,'coltrane_link_archive_index'),
    (r'^links/(?P<year>\d{4})/$', 'archive_year', link_info_dict_year,'coltrane_link_archive_year'),
    (r'^links/(?P<year>\d{4})/(?P<month>\w{3})/$', 'archive_month', link_info_dict,'coltrane_link_archive_month'),
    (r'^links/(?P<year>\d{4})/(?P<month>\w{3})/(?P<day>\d{2})/$', 'archive_day', link_info_dict,'coltrane_link_archive_day'),
    (r'^links/(?P<year>\d{4})/(?P<month>\w{3})/(?P<day>\d{2})/(?P<slug>[-\w]+)/$', 'object_detail', link_info_dict,'coltrane_link_detail'),   

)

urlpatterns += patterns('',
     (r'^categories/$', 'django.views.generic.list_detail.object_list',{'query_set':Category.objects.all()}),
     (r'^categories/(?P<slug>[-\w]+)/$', 'coltrane.views.category_detail'),
)
