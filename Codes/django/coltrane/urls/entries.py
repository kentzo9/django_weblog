from django.conf.urls.defaults import *

from coltrane.models import Entry

entry_info_dict = {
    'queryset': Entry.live.all(),
    'date_field': 'pub_date',
    }
entry_info_dict1 = {
    'queryset': Entry.live.all(),
    'date_field': 'pub_date',
    'template_name':'coltrane/entry_index.html',
    'template_object_name':'entry_list',
    }

urlpatterns = patterns('',(r'^$', 'coltrane.views.track_archive',{}, 'coltrane_entry_archive_index'))
urlpatterns += patterns('django.views.generic.date_based',
    # (r'^$', 'archive_index', entry_info_dict1, 'coltrane_entry_archive_index'),
     (r'^(?P<year>\d{4})/$', 'archive_year', entry_info_dict,'coltrane_entry_archive_year'),
     (r'^(?P<year>\d{4})/(?P<month>\w{3})/$',
      'archive_month',entry_info_dict,'coltrane_entry_archive_month'),
     (r'^(?P<year>\d{4})/(?P<month>\w{3})/(?P<day>\d{2})/$',
      'archive_day',entry_info_dict,'coltrane_entry_archive_day'),
    # (r'^(?P<year>\d{4})/(?P<month>\w{3})/(?P<day>\d{2})/(?P<slug>[-\w]+)/$',
    #  'object_detail',entry_info_dict,'coltrane_entry_detail'),
)

urlpatterns +=patterns('', 
     (r'^(?P<year>\d{4})/(?P<month>\w{3})/(?P<day>\d{2})/(?P<slug>[-\w]+)/$',
      'coltrane.views.entry_detail',{},'coltrane_entry_detail')
              )
