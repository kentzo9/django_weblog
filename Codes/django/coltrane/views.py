from django.shortcuts import render_to_response, get_object_or_404
from coltrane.models import Entry, Category
from django.views.generic.list_detail import object_list
from django.views.generic.date_based import archive_index,object_detail
from django.template import RequestContext

def entries_index(request):
    return render_to_response('coltrane/entry_index.html', {'entry_list' : Entry.objects.all()})

#This is not using the object_detail() from views.generic.date_base.py
def entry_detail(request,year,month,day,slug):
  import datetime,time
  date_stamp = time.strptime(year+month+day, "%Y%b%d")
  pub_date = datetime.date(*date_stamp[:3])
  print pub_date.year, pub_date.month, pub_date.day
  entry = get_object_or_404(Entry, pub_date__year= pub_date.year,
                            pub_date__month= pub_date.month,
                            pub_date__day= pub_date.day,
                            slug = slug,
                            status = Entry.LIVE_STATUS)
  ##no need to check if entry is found, if not found, HTTP_404 will be raised.
  if not entry.enable_comments and not request.user.is_authenticated():
    return HttpResponseRedirect('/accounts/login/?next=%s' % request.path)
  return render_to_response('coltrane/entry_detail.html', {'object':entry},context_instance=RequestContext(request))

# can use the login required decorator if desired.
#from django.contrib.auth.decorators import login_required
#@login_required
#def entry_detail(request, year, month, day, slug):
#   # if (request.user.is_authenticated()):
#   #    print "This is authenticated user"
#   #    print "current_user is '%s'" % (current_user.get_full_name())
#   # elif(request.user.is_anonymous()):
#   #    print "This is anonymous user"
#    if not request.user.is_authenticated():
#       return HttpResponseRedirect('/accounts/login/?next=%s' % request.path)
#    return object_detail(request,year,month,day,Entry.live.all(),'pub_date',slug=slug )

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
    current_user=request.user
    if (current_user.is_authenticated()):
       print "This is authenticated user"
       print "current_user is '%s'" % (current_user.get_full_name())
    elif(current_user.is_anonymous()):
       print "This is anonymous user"
    category = get_object_or_404(Category, slug=slug)
    #return render_to_response('coltrane/category_detail.html',
    #                          { 'object_list': category.entry_set.all(),
    #                             'category': category })
    return object_list(request, queryset=category.live_entry_set(),extra_context={'category':category} )

def track_archive(request):
   tracks = Entry.live.all()
   archive = {}

   date_field = 'pub_date'

   years = tracks.dates(date_field, 'year')[::-1]
   for date_year in years:
       months = tracks.filter(pub_date__year=date_year.year).dates(date_field, 'month')
       archive[date_year] = months

   archive = sorted(archive.items(), reverse=True)

   return archive_index(
        request,
        date_field=date_field,
        queryset=tracks,
        template_name='coltrane/entry_index.html',
        template_object_name='entry_list',
        extra_context={'archive': archive},
   )

#from django.contrib.auth import authenticate
from django.contrib.auth import login,logout,authenticate
from django.views.generic.simple import direct_to_template
from django.http import HttpResponseRedirect, Http404
def mylogin(request):
  if request.method == 'POST':
    user = authenticate(username=request.POST['username'], password=request.POST['password'])
    if user is not None:
      if user.is_active:
        login(request, user)
        # success
        return HttpResponseRedirect('/weblog')
      else:
        # disabled account
        return direct_to_template(request, 'inactive_account.html')
    else:
      # invalid login
      return direct_to_template(request, 'registration/invalid_login.html')
      
def mylogout(request):
  logout(request)
  return HttpResponseRedirect('/weblog')
  #return direct_to_template(request, 'logged_out.html')
