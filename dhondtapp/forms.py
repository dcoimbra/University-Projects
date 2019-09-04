from django import forms
from django.forms import formset_factory
import dhondtapp.dhondt as dhondt


class VotesForm(forms.Form):
    votes = forms.IntegerField(label="")

VotesFormSet = formset_factory(VotesForm, extra=len(dhondt.PARTIDOS))
