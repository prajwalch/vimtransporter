" Author        : Prajwal Chapagain <prajjwal2058@gmail.com>
" Date          : Friday Aug 06, 2021 14:46:44 NPT
" License       : MIT

func MsgReceiver(channel, msg)
    echo 'Received: ' .a:msg
endfunc

func OpenTransporterSock()
    if has('channel')
        if exists('g:channel')
            return
        endif

        let g:channel = ch_open('localhost:2058', {"callback":"MsgReceiver"})
    endif
endfunc

if exists('g:auto_start_transporter') && g:auto_start_transporter == 'true'
    call OpenTransporterSock()
endif

func SendPingMsg()
    if exists('g:channel')
        call ch_sendexpr(g:channel, "PING", {"callback":"MsgReceiver"})
    endif
endfunc

nnoremap <C-p> :call SendPingMsg()<CR>
