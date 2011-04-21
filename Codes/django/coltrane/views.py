from django.shortcuts import render_to_response, get_object_or_404
from coltrane.models import Entry, Category
from django.views.generic.list_detail import object_list
from django.views.generic.date_based import archive_index

def entries_index(request):
    return render_to_response('coltrane/entry_index.html', {'entry_list' : Entry.objects.all()})

def entry_detail(request,year,month,day,slug):
   import datetime,time
   date_stamp = time.strptime(year+month+day, "%Y%b%d")
   pub_date = datetime.date(*date_stamp[:3])
   entry = get_object_or_404(Entry, pub_date__year= pub_date.year,
                             pub_date__month= pub_date.month,
                             pub_date__day= pub_date.day,
                             slug = slug)
   return render_to_response('coltrane/entry_detail.html', {'entry':entry})

def category_list(request):
    return render_to_response('coltrane/category_list.html', {'object_list' : Category.objects.all()})

##The following technique can be used to send download file to the
##browser. The template_name here can be any text file, not necessary
##a valid html file. change the template_name to arbitary text file.
def category_list_plaintext(request):
   print "ZhangLI"
   response = object_list(request,
                          queryset = Category.objects.all(),
                          mimetype = 'text/plain',                                           template_name = 'coltrane/category_list.html')
   response["Content-Disposition"] = "attachment; filename=category_list.txt"
   return response

##Displaying the list of entries under certain category passed in as slug
def category_detail(request, slug):
    category = get_object_or_404(Category, slug=slug)
    #return render_to_response('coltrane/category_detail.html',
    #                          { 'object_list': category.entry_set.all(),
    #                             'category': category })
    return object_list(request, queryset=category.live_entry_set(),extra_context={'category':category} )

def track_archive(request):
   tracks = Entry.objects.live()
   archive = {}

   date_field = 'pub_date'

   years = tracks.dates(date_field, 'year')[::-1]
   for date_year in years:
       months = tracks.filter(date__year=date_year.year).dates(date_field, 'month')
       archive[date_year] = months

   archive = sorted(archive.items(), reverse=True)

   return date_based.archive_index(
        request,
        date_field=date_field,
        queryset=tracks,
        extra_context={'archive': archive},
   )
