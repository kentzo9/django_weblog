# Create your views here.
from django.http import HttpResponse, HttpResponseRedirect
from django.template import loader, Context
from django.contrib.flatpages.models import FlatPage
from django.shortcuts import render_to_response

def search(request):
   #query = request.GET['q']
   #results = FlatPage.objects.filter(content__icontains=query)
   #template = loader.get_template('search/search.html')
   #context = Context({'query': query, 'results':results})
   #response = template.render(context)
   query = request.GET.get('q','')
   keyword_results =[]
   results = []
   if query:
      keyword_results = FlatPage.objects.filter(searchkeyword__keyword__in=query.split()).distinct()
      if keyword_results.count() == 1:
         return HttpResponseRedirect(keyword_results[0].get_absolute_url())
      results = FlatPage.objects.filter(content__icontains=query)
   return render_to_response('search/search.html',
                              {'query':query, 'results':results, 'keyword_results':keyword_results})
