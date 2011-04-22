from django.conf.urls.defaults import *

# Uncomment the next two lines to enable the admin:
from django.contrib import admin
admin.autodiscover()

from coltrane.feeds import LatestEntriesFeed, CategoryFeed
feeds = {'entries': LatestEntriesFeed }
feed2s = {'categories': CategoryFeed }

urlpatterns = patterns('',
    # Example:
    # (r'^cms/', include('cms.foo.urls')),

    # Uncomment the admin/doc line below to enable admin documentation:
    # (r'^admin/doc/', include('django.contrib.admindocs.urls')),

    # Uncomment the next line to enable the admin:
    (r'^admin/', include(admin.site.urls)),
    (r'^tiny_mce/(?P<path>.*)$', 'django.views.static.serve',
                 { 'document_root': '/home/kentzo/django_weblog/javascript/tinymce/jscripts/tiny_mce/' }),
    (r'^search/$', 'cms.search.views.search'),
    (r'^weblog/categories/', include('coltrane.urls.categories')),
    (r'^weblog/links/', include('coltrane.urls.links')),
    (r'^weblog/tags/', include('coltrane.urls.tags')),
    (r'^weblog/', include('coltrane.urls.entries')),
    (r'^comments/',include('django.contrib.comments.urls')),
    (r'^feeds/(?P<url>entries)/$','django.contrib.syndication.views.feed',{'feed_dict':feeds}),
    (r'^feeds/(?P<url>categories/.*)/$','django.contrib.syndication.views.feed',{'feed_dict':feed2s}),
    (r'^site_media/(?P<path>.*)$', 'django.views.static.serve',{'document_root': '/home/kentzo/django_weblog/media/'}),
    (r'^accounts/login/$', 'django.contrib.auth.views.login'),
    (r'^accounts/logout/$', 'django.contrib.auth.views.logout'),
    (r'^login/$', 'coltrane.views.mylogin'),
    (r'^logout/$', 'coltrane.views.mylogout'),
    (r'', include('django.contrib.flatpages.urls')),
)
