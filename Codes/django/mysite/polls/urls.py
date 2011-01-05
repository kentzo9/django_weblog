from django.conf.urls.defaults import *
#from django.views.generic import DetailView, ListView
from polls.models import Poll

#urlpatterns = patterns('polls.views',
#    (r'^$', 'index'),
#    (r'^(?P<poll_id>\d+)/$', 'detail'),
#    (r'^(?P<poll_id>\d+)/results/$', 'results'),
#    (r'^(?P<poll_id>\d+)/vote/$', 'vote'),
#)

info_dict = {
    'queryset': Poll.objects.all(),
}

urlpatterns = patterns('',
    (r'^$', 'django.views.generic.list_detail.object_list', info_dict),
    (r'^(?P<object_id>\d+)/$', 'django.views.generic.list_detail.object_detail', info_dict),
    url(r'^(?P<object_id>\d+)/results/$', 'django.views.generic.list_detail.object_detail', dict(info_dict, template_name='polls/results.html'), 'poll_results'),
    (r'^(?P<poll_id>\d+)/vote/$', 'polls.views.vote'),
)

# urlpatterns = patterns('',
#     (r'^$',
#         ListView.as_view(
#             model=Poll,
#             context_object_name='latest_poll_list',
#             template_name='polls/index.html')),
#     (r'^(?P<pk>\d+)/$',
#         DetailView.as_view(
#             model=Poll,
#             template_name='polls/detail.html')),
#     url(r'^(?P<pk>\d+)/results/$',
#         DetailView.as_view(
#             model=Poll,
#             template_name='polls/results.html'),
#         name='poll_results'),
#     (r'^(?P<poll_id>\d+)/vote/$', 'polls.views.vote'),
# )
