from django.urls import path
from dhondtapp.views import home_view

urlpatterns = [
    path('', home_view, name='home')
]
