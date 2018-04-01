#!/bin/sh

cmd_exists() {
    if [ -z "$ExistChecker" ]; then
        FindExistChecker
    fi
    case "$ExistChecker" in
        'command')
            command -v "$1" >/dev/null 2>&1
            return $?
            ;;
        'type')
            type "$1" >/dev/null 2>&1
            return $?
            ;;
        'shell')
            $ExistCheckerSh -c "command -v $1 >/dev/null 2>&1" >/dev/null 2>&1
            return $?
            ;;
        'csh')
            csh -c "which $1 >& /dev/null" >/dev/null 2>&1
            return $?
            ;;
        'which')
            which "$1" >/dev/null 2>&1
            return $?
            ;;
    esac

    return 255
}


# Yes, all of the redirections are actually necessary. Most shells don't need
# them, but the most ancient and terrible of shells will insist on echoing
# errors unless you put the command in a subshell and redirect that too.
FindExistChecker() {
    if (command -v ls >/dev/null 2>&1) >/dev/null 2>&1; then
        ExistChecker='command'
    elif (type ls >/dev/null 2>&1) >/dev/null 2>&1; then
        ExistChecker='type'
    else
        for shell in 'sh' 'bash' 'dash' 'ksh' 'mksh' 'pdksh' 'ash' 'busybox ash' 'zsh'; do
            if ($shell -c 'command -v ls >/dev/null 2>&1' >/dev/null 2>&1) >/dev/null 2>&1; then
                ExistChecker='shell'
                ExistCheckerSh="$shell"
                break
            fi
        done
    fi
    if [ -z "$ExistChecker" ]; then
        if (csh -c 'which ls >& /dev/null' >/dev/null 2>&1) >/dev/null 2>&1; then
            ExistChecker='csh'
        elif (which ls >/dev/null 2>&1) >/dev/null 2>&1; then
            ExistChecker='which'
        fi
    fi

    if [ -z "$ExistChecker" ]; then
        cat <<__EOF__
Your shell does not support "command -v", you do not have 'which' installed,
and you have no other shells installed (in a sane location) that support any
equivalent. Amazing. I could try to revert to something like testing whether
'cmd --version' succeeds, but instead, out of spite, this is a fatal error.
__EOF__
        exit 255
    fi
}

cmd_exists "$@"
