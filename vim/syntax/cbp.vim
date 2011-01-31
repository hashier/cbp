" Vim syntax file
" Language:		CBP
" Info:			$Id: cbp.vim,v 1.000 Mon Jan 31 01:22:30 CET 2011 Exp $
" ----------------------------------------------------------------------------
"
" ----------------------------------------------------------------------------

if exists("b:current_syntax")
  finish
endif

" Operators
" syn match  cbpOperator	 "\%([~!^&|*/%+-]\|\%(class\s*\)\@<!<<\|<=>\|<=\|\%(<\|\<class\s\+\u\w*\s*\)\@<!<[^<]\@=\|===\|==\|=\~\|>>\|>=\|=\@<!>\|\*\*\|\.\.\.\|\.\.\|::\)"

" Numbers
syn match cbpInteger	"\<\%(\d\+\)\>"									display
syn match cbpFloat	"\<\%(\d\*\)\.\d\+\>"								display

" doesn't work yet
" syn match cbpOffset	"@\s+\<\%(\d\+\)\>"								display

" Keywords
syn keyword cbpFunction		func abi_c
syn keyword cbpControl		if else while for break call
syn keyword cbpVar		local var
syn keyword cbpTypeDecl		type struct []
syn keyword cbpType		uint8 int8 uint16 int16 uint32 int32 float32 float64 void
syn keyword cbpKeyword		super undef yield
syn keyword cbpPseudoVariable	nil self __FILE__ __LINE__
syn keyword cbpBeginEnd	BEGIN END

" Comments and Documentation
syn keyword cbpTodo	      FIXME NOTE TODO OPTIMIZE XXX contained
syn match   cbpComment        "#.*" contains=cbpTodo,@Spell

hi def link cbpFunction			Function
hi def link cbpControl			Statement
hi def link cbpInteger			Number
hi def link cbpFloat			Float
hi def link cbpType			Type
hi def link cbpTypeDecl			Type
hi def link cbpVar			Type
hi def link cbpOffset			Function
hi def link cbpKeyword			Keyword
hi def link cbpOperator			Operator

hi def link cbpComment			Comment
hi def link cbpTodo			Todo

let b:current_syntax = "cbp"

" vim: nowrap sw=2 sts=2 ts=8 noet ff=unix:
