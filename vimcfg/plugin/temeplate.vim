"set header automatically
autocmd BufNewFile *.swift,*.sh,*.java,*.py,*.go,*.php,*.cs exec ":call SetHeader()"
"Go to the end line of the new document
autocmd BufNewFile * normal G

func! <SID>GetYear()
    return strftime("%Y")
endfunc


func! <SID>GetFileName()
    let fname = expand("%")
    return fname
endfunc


func! <SID>GetUserName()
    let home = $HOME
    let user = matchstr(home, '[^/\\]\+$')
    " let user = "Roger Luo"
    return user
endfunc


func! <SID>GetEmail()
    return "tjrogertj@gmail.com"
endfunc


func! <SID>GetDate()
    let date = strftime("%Y-%m-%d %H:%M:%S")
    return date
endfunc


func! <SID>GetCopyright()
    return "Copyright © ".<SID>GetYear()." ".<SID>GetUserName().". All rights reserved."
endfunc


func! <SID>GetAuthor()
    return <SID>GetUserName()."[".<SID>GetEmail()."]"
endfunc


func! StarLine()
    return "\###########################################################################"
endfunc


func! Copyright()
    return "\# ".<SID>GetCopyright()
endfunc


func! <SID>Author()
    return "Author: " .<SID>GetAuthor()
endfunc


func! Author()
    return "\# ".<SID>Author()
endfunc


func! <SID>CreatedAt()
    return "Created At: " .<SID>GetDate()
endfunc


func! CreatedAt()
    return "\# ".<SID>CreatedAt()
endfunc


func! <SID>File()
    return "File: " .<SID>GetFileName()
endfunc


func! File()
    return "\# ".<SID>File()
endfunc


func! <SID>Description()
    return "desc:"
endfunc


func! Description()
    return "\# ".<SID>Description()
endfunc


func! <SID>VimSetting()
    return "vim: set ts=4 sw=4 sts=4 tw=100"
endfunc


func! VimSetting()
    return "\# ".<SID>VimSetting()
endfunc


"SetHeader definition, add header to file automatically
func! SetHeader()
    if &filetype == 'php' || &filetype == 'go' || &filetype == 'java' || &filetype == 'cs' || &filetype == 'swift'
        if &filetype == 'php'
            call setline(1, "<?php")
        elseif &filetype == 'go'
            call setline(1, "")
        elseif &filetype == 'java'
            call setline(1, "")
        elseif &filetype == 'swift'
            call setline(1, "\#!/usr/bin/swift")
        endif
        call append(1, "/***************************************************************************")
        call append(2, " ".<SID>GetCopyright())
        call append(3, " **************************************************************************/")
        call append(4, "")
        call append(5, "/**")
        call append(6, " * ".<SID>File())
        call append(7, " * ".<SID>Author())
        call append(8, " * ".<SID>CreatedAt())
        call append(9, " * ".<SID>Description())
        call append(10, " */")
        call append(11, "/* ".<SID>VimSetting()." */")
        call append(12, "")
        "filetype sh
    elseif &filetype == 'sh' || &filetype == 'python'
        if &filetype == 'sh'
            call setline(1, "\#!/bin/bash")
            call append(1, "\# coding: utf-8")
        else
            call setline(1, "\#!/usr/bin/env python3")
            call append(1, "\# -*- coding: utf-8 -*-")
        endif
        call append(2, StarLine())
        call append(3, Copyright())
        call append(4, StarLine())
        call append(5, Author())
        call append(6, CreatedAt())
        call append(7, File())
        call append(8, Description())
        call append(9, VimSetting())
        call append(10,"")
    endif
endfunc
