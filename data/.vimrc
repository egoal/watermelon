" ref https://zhuanlan.zhihu.com/p/605261047

" Adjust the terminal and vim color
let &t_ut=''

" Enable 256 colors
set t_Co=256
"hi CursorLine term=bold cterm=bold ctermbg=235
"hi CursorLine term=bold cterm=bold ctermbg=235
hi CursorLine ctermfg=NONE ctermbg=235 cterm=NONE
hi CursorLineNr ctermfg=white ctermbg=235 cterm=NONE guibg=NONE guifg=yellow

" Set the leader key
let mapleader=" "

" Is not compatible with vi
set nocompatible

" Disable the mouse
set mouse=a

" Set the encoding format to UTF-8
set encoding=utf-8

" Syntax highlighting
syntax on

" show linenumber and relative linenumber
set number
" set relativenumber

" Highlight current line
set cursorline
"set ruler
set colorcolumn=80,120
highlight ColorColumn ctermbg=0 guibg=lightgrey

" Bottom display the mode info.
set showmode

" Enable indentation
set autoindent
set smartindent
set smarttab
set expandtab
set shiftwidth=4
set tabstop=4
set softtabstop=4

" Intelligently adapt the indentation of different types of files
filetype plugin on
filetype indent on
filetype plugin indent on

" When scrolling vertically, the cursor is kept 5 rows away from the top/bottom
set scrolloff=5

" Automatically switch the working directory
set autochdir

" Turn off automatic line wrapping
set nowrap

" Solve the problem that the Backspace key cannot be used in some cases
set backspace=indent,eol,start

" Command auto-completion related
set wildmenu

" About searching and highlighting
set hlsearch
set incsearch
exec "nohlsearch"
set ignorecase
set smartcase

" Alleviate the lag when opening single-line large files
set lazyredraw
set ttyfast

"No backup file
set nobackup
set noswapfile
set nowritebackup

" Custom keymap
map s <nop>
map S :w<CR>
map Q :q<CR>
map R :source $MYVIMRC<CR>

noremap J 10j
noremap K 10k
noremap <LEADER><CR> :nohlsearch<CR>

" Configure the status bar
set laststatus=2
hi statusline guibg=DarkGrey ctermfg=8 guifg=White ctermbg=15
set statusline=\ @simple " user leader
set statusline+=\ \|\|\ [FileName:\ %f]
set statusline+=%=
set statusline+=\ %y
set statusline+=\ %{&fileencoding?&fileencoding:&encoding}
set statusline+=\[%{&fileformat}\]
set statusline+=\ %p%%
set statusline+=\ [%l:%c]

" Reopen the last edited position in files
if has("autocmd")
  autocmd BufReadPost *
    \ if line("'\"") > 1 && line("'\"") <= line("$") |
    \   exe "normal! g`\"" |
    \ endif
endif
