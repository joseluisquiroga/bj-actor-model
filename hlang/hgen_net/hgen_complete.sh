#/usr/bin/env bash

_hgen_lognet_completions()
{
	export COMP_CWORD
	eval $COMP_LINE ++
	mapfile -t COMPREPLY < <( cat autocomplete_choices.txt )
	echo "" > autocomplete_choices.txt
}

complete -F _hgen_lognet_completions hgen_lognet

