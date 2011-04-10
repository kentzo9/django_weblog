from django import template
from coltrane.models import Entry, Link
from django.db.models import get_model
from tagging.utils import get_tag
from tagging.models import Tag, TaggedItem

#this is the functions that tie to the tag, it must accept parser and token
#it must return a template.node object
def do_latest_entries(parser, token):
   return LatestEntriesNode()

#subclas of template.Node which must have a render method which return a string
class LatestEntriesNode(template.Node):
   def render(self, context):
      context['latest_entries'] = Entry.live.all()[:5]
      return ''
    
## This is the more general function compare to the above
def do_latest_content(parser, token):
   '''get_latest_content appname.modelname #object as varname '''
   bits = token.split_contents()
   if len(bits) != 5:
      raise template.TemplateSyntaxError("'get_latest_content' takes exactly four arguments")
   model_args = bits[1].split('.')
   if len(model_args) != 2:
      raise template.TemplateSyntaxError("First argument to 'get latest_content' must be an 'application name'.'model name' string")
   model = get_model(*model_args)
   if model is None:
      raise template.TemplateSyntaxError("'get_latest_content' tag got an invalid model: %s" % bits[1])
   return LatestContentNode(model, bits[2], bits[4])
  
class LatestContentNode(template.Node):
   def __init__(self, model, num, varname):
       self.model = model
       self.num = int(num)
       self.varname = varname

#Whenever you don't know in advance which model you'll be working with
#(as in this case, and in most cases when you're using get_model()), it's a good
#idea to use _default_manager. When a model has multiple managers, or defines
#a single custom manager that's not named objects, trying to query through the
#objects attribute can be dangerous. That might not be the manager that queries
#should go through (as in the case of Entry with its special live manager), and
#in fact,  objects might not even exist. Remember that when a model has a
#custom manager, Django doesn't automatically set up the objects manager on it,
#so trying to access objects  might raise an exception.
   def render(self, context):
       context[self.varname] = self.model._default_manager.all()[:self.num]
       return ''

register = template.Library()
register.tag('get_latest_entries', do_latest_entries)
register.tag('get_latest_content', do_latest_content)

@register.filter
def cut(value, arg):
   "Removes all values of arg from the given string"
   return value.replace(arg, '')

@register.filter
def tag_in_entry(value):
   "Removes all values of arg from the given string"
   if isinstance(value, Tag):
      queryset = TaggedItem.objects.get_by_model(Entry,value)
      return queryset.count()
   else:
      return False

@register.filter
def tag_in_link(value):
   "Removes all values of arg from the given string"
   if isinstance(value, Tag):
      queryset = TaggedItem.objects.get_by_model(Link,value)
      return queryset.count()
   else:
      return False

@register.filter
def is_list_not_empty(list):
   if type(list).__name__=='list':      
      return len(list) > 0
   else:
      return False
