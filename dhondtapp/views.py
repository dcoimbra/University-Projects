from django.shortcuts import render, redirect
import dhondtapp.dhondt as dhondt
import dhondtapp.forms as forms

# Create your views here.


def home_view(request):

    if request.method == 'POST':

        mandates = int(request.POST['mandates'])
        votesformset = forms.VotesFormSet(request.POST)

        if votesformset.is_valid():
            voting_results = tuple(form.cleaned_data.get('votes')
                                   for form in votesformset)

            mandates_result = dhondt.mandatos(mandates, voting_results)

    else:
        mandates_result = None
        votesformset = forms.VotesFormSet()

    context = {
        "party_form": zip(votesformset, dhondt.PARTIDOS),
    }

    if mandates_result is not None:
        context['party_mandates'] = zip(mandates_result, dhondt.PARTIDOS)

    return render(request, "home.html", context)
