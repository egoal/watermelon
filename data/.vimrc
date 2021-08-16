
" get plug.vim
if empty(glob('~/.vim/autoload/plug.vim'))
	silent !curl -fLo ~/.vim/autoload/plug.vim --create-dirs 
		\ https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim
	autocmd VimEnter * PlugInstall --sync| source $MYVIMRC
endif

""" plugins
call plug#begin('~/.vim/plugged')
" theme
Plug 'jacoborus/tender.vim'
Plug 'rakr/vim-one'
Plug 'iCyMind/NeoSolarized'
Plug 'morhetz/gruvbox'
Plug 'crusoexia/vim-monokai'

" ycm
Plug 'Valloric/YouCompleteMe' 

" 
Plug 'jiangmiao/auto-pairs'
Plug 'lrvick/Conque-Shell'
Plug 'scrooloose/nerdtree'
Plug 'kien/ctrlp.vim'
Plug 'jbgutierrez/vim-better-comments'
Plug 'vim-airline/vim-airline'

call plug#end()

""" plugin setting
let g:NERDTreeShowHidden=0
let g:NERDTreeWinSize=32

" common
set nocompatible

set mouse=a

set smarttab
set expandtab
set tabstop=4
set shiftwidth=4
set autoindent
set cindent
set number
set ruler
set cmdheight=2
set showcmd
set backspace=indent,eol,start
set foldenable
set foldmethod=manual

" enable syntax highlighting
syntax enable

set encoding=utf8

" set termguicolors
" colorscheme elflord 
colorscheme desert

set colorcolumn=80
highlight ColorColumn ctermbg=0 guibg=lightgrey

""" key maps
" window switch
map <C-j> <C-W>j
map <C-k> <C-W>k
map <C-h> <C-W>h
map <C-l> <C-W>l

" nerdtree
map <C-B> :NERDTreeToggle<cr>


