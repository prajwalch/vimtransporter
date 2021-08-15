" Author        : Prajwal Chapagain <prajjwal2058@gmail.com>
" Date          : Friday Aug 06, 2021 14:46:44 NPT
" License       : MIT

func! MsgReceiver(channel, msg)
    echo 'Server response: ' .a:msg
endfunc

func! HasChOpened()
    if exists('g:channel')
        if ch_status(g:channel) == "open"
            return 1
        endif
    endif

    return 0
endfunc

func! OpenTransporterSock()
    if exists('g:auto_start_transporter')
        if g:auto_start_transporter == 'false'
            return
        endif

        if HasChOpened()
            return
        endif

        let g:channel = ch_open('localhost:2058', {'callback': 'MsgReceiver'})
    endif " exists('g:auto_start_transporter')
endfunc

func! SendPingMsg()
    if HasChOpened()
        call ch_sendexpr(g:channel, "PING", {"callback":"MsgReceiver"})
    endif " if HasChOpened()
endfunc

func! FindAndReplace(txt, pat, sub)
    let l:mod_txt = substitute(a:txt, a:pat, a:sub, "g")
    return l:mod_txt
endfunc

" ' -> TPTERsqo
" " -> TPTERdqo
" $ -> TPTERdsi

let g:single_quote_holder = "TPTERsqo"
let g:double_quote_holder = "TPTERdqo"
let g:dollar_sign_holder = "TPTERdsi"

let g:single_quote = "'"
let g:double_quote = "\""
let g:dollar_sign = "$"

func! EncodeYankedText(txt)
    let l:mod_txt = a:txt

    " check for single quote
    if count(a:txt, g:single_quote) > 0
        let l:mod_txt = FindAndReplace(a:txt, g:single_quote, g:single_quote_holder)
    endif

    " check for double quote
    if count(a:txt, g:double_quote) > 0
        let l:mod_txt = FindAndReplace(a:txt, g:double_quote, g:double_quote_holder)
    endif

    " check for dollar sign
    if count(a:txt, g:dollar_sign) > 0
        let l:mod_txt = FindAndReplace(a:txt, g:dollar_sign, g:dollar_sign_holder)
    endif

    return l:mod_txt
endfunc

func! DecodeYankedText(txt)
    let l:mod_txt = a:txt

    " check for single quote
    if count(a:txt, g:single_quote_holder) > 0
        let l:mod_txt = FindAndReplace(a:txt, g:single_quote_holder, g:single_quote)
    endif

    " check for double quote
    if count(a:txt, g:double_quote_holder) > 0
        let l:mod_txt = FindAndReplace(a:txt, g:double_quote_holder, g:double_quote)
    endif

    " check for dollar sign
    if count(a:txt, g:dollar_sign_holder) > 0
        let l:mod_txt = FindAndReplace(a:txt, g:dollar_sign_holder, g:dollar_sign)
    endif

    return l:mod_txt
endfunc

func! GetYankedText()
    let l:yanked_txt = getreg("0")
    let l:encoded = EncodeYankedText(l:yanked_txt)

    return l:encoded
endfunc

func! SendYankedText()
    if exists('g:auto_send_yanked_txt')
        if g:auto_send_yanked_txt == 'false'
            return
        endif
        let l:yanked_txt = GetYankedText()
        let l:msg = join(["$ex::call RecvYankedText('",l:yanked_txt,"')$"], "")

        if HasChOpened()
            call ch_sendexpr(g:channel, l:msg)
        endif " if HasChOpened()
    endif " g:auto_send_yanked_txt
endfunc

func! RecvYankedText(enc_yanked_txt)
    let l:yanked_txt = DecodeYankedText(a:enc_yanked_txt)
    call setreg("0", "", "V")
    call setreg("0", l:yanked_txt, "V")
endfunc


autocmd TextYankPost * call SendYankedText()
autocmd BufNewFile,BufRead * call OpenTransporterSock()
autocmd SourcePost .vimrc,.vim call OpenTransporterSock()

nnoremap <C-p> :call SendPingMsg()<CR>
