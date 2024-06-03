#!/bin/sh

# SPDX-FileCopyrightText: 2023 Anthony Loiseau <anthony.loiseau@allcircuits.com>
# SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

### This script simplifies the call of `reuse annotate` command dedicated to
### the update or addition of copyright headers in files, especially with
### auto-detection of author name and email (from git configuration) and
### the selection of our specific ACT license: LicenseRef-ALLCircuits-ACT-X.Y.
###
### Note that for repo to pass "reuse lint" checks, ACT license file must be
### added in the repo root LICENSES folder. This is out of this script scope.
###
### This script serves two purposes:
###
### By default it updates headers of git staged files (updated or added files).
### This prevent adding headers in temporary files. It does this using current
### git configuration user name and email, current date and very-known ACT
### license name.
###
### Given the "init" command, it parses history of all git versionned files
### (authors, date) and generates copyright headers matching file history.
### A repo should only be initialized once otherwise extra copyrights lines
### may be inserted matching first initialisation year and author.
###
### This script requires reuse tool to be accessible from PATH.
### See reuse sources README file for installation procedure
### (https://git.fsfe.org/reuse/tool):
### Here is a modified, tested-working installation sequence for Windows:
###  - %PYTHON3% -m pip install pipx
###  - %PYTHON3% -m pipx install reuse
###  - %PYTHON3% -m pipx ensurepath
###
### See REUSE website for more information on this tool: https://reuse.software
###
## USAGE: $0 [global-opts] [command] [cmd-opts/path]
##
## Global options:
##  -h, --help        Show usage
##  -hh, --help-full  Show deeper help
##
## Commands:
##  help        Show usage
##              Command options: none
##
##  help-full   Show deeper help
##              Command options: none
##
##  init        Force the adding of the headers in all the files in the repo.
##              This is useful when the repo is an old one and no files have
##              SPDX info in their header.
##              Command options:
##                 path (optional): path glob to init, default to "."
##                                  can be used to act on a sub-folder ("foo/")
##                                  or a given file type for example ("*.foo")
##
##  update      Default command.
##              Update copyright headers of modified file.
##              Author is taken from current git configuration,
##              and current year is used.
##              Command options:
##                 path (optional): path glob to update, default to "."
##                                  can be used to act on a sub-folder ("foo/")
##                                  or a given file type for example ("*.foo")
##                 In the future, it may be useful to let caller choose
##                 between processing staged only files as currently enforced
##                 or worktree modified files (not yet supported).
###
### Advanced usage:
###
### mailmap:
###   Especially with older projects, a git ".mailmap" file can be useful.
###   When git finds this file in the repo root, it uses it to fix some authors
###   name and emails. This is very useful for reuse to not mistakenly threat
###   authors as different when :
###   - a commit has been done with a wrong or slightly different name case:
###        since reuse threats "Foo Bar" and "Foo BAR" as two different authors
###   - a commit had mistakenly been done with a bad or private email
###   - a commit had been done with an old, deprecated corporate email
###   See https://git-scm.com/docs/gitmailmap
###
###   When the triple (first name, last name, email) of an author is always the
###   same, reuse properly merges copyright headers. Otherwise, reuse threat it
###   as a different authors and create a duplicate lines.
###   Therefore, please always set your git user.name configuration to
###      First name Last Name
###   or fix it through a .mailmap.
###

# Globally enable a few optional/extra shellcheck checks
# shellcheck enable=check-unassigned-uppercase  # SC2154
# shellcheck enable=check-extra-masked-returns  # SC2155 SC2312
# shellcheck enable=quote-safe-variables        # SC2248
# shellcheck enable=require-variable-braces     # SC2250

# Globally disable a few other ones
# shellcheck disable=SC3043  # POSIX sh does not defines "local" keyword


set -e
set -u
# shellcheck disable=SC3040
set -o pipefail


REPO_DIR="$(git rev-parse --show-toplevel)"
readonly REPO_DIR

# Should specific file or folder be targetted (default to current subtree)
OPT_TARGET="."

# The ACT license name which will be used in all our files
ACT_LICENSE_NAME="LicenseRef-ALLCircuits-ACT-1.1"

# The folder where the licenses are stored
LICENSES_FOLDER="${REPO_DIR}/LICENSES"

# The ACT license file path
ACT_LICENSE_FILE_PATH="${LICENSES_FOLDER}/${ACT_LICENSE_NAME}.txt"

# Variable holding a simple new line character, the POSIX way
NL="$(printf '\nx')"; NL="${NL%x}"
readonly NL

# Small helpers --------------------------------------------------------

# Print arguments tostderr and die
# $*: Error text to be printed
die() {
    printf "%s" "$*" >&2
    exit 1
}

# Print script help
# $1: optional verbosity (minimal leading hashes count), defaults to 2
print_help() {
    grep "^#\{2,${1:-2}\}\( \|\$\)" "$0" | sed 's/^#* \?//' | sed "s,\$0,$0,"
}

# Ensure $1 appears to be a valid author name
test_author_name_validity() {
    case ${1} in
        *\ *)
            # At least two tokens, OK
            true
            ;;

        *)
            false
            ;;
    esac
}

# Ensure $1 appears to be a valid author email
test_author_email_validity() {
    case ${1} in
        *@allcircuits.com)
            true
            ;;
        *@allcircuits-technologies.com)
            true
            ;;
        *@actech-ingenierie.fr)
            true
            ;;

        *)
            false
            ;;
    esac
}

# Print current git author "Full Name <email>"
get_copyright_info() {
    local author_name="${GIT_AUTHOR_NAME:-}"
    local author_email="${GIT_AUTHOR_EMAIL:-}"

    if test -z "${author_name}"
    then
        author_name="$(git config --default '' --get author.name)"

        if test -z "${author_name}"
        then
            author_name="$(git config --default '' --get user.name)"

            if test -z "${author_name}"
            then
                die "We can't get the author name from git, please fills your `
                    `git config file or use the env variable: GIT_AUTHOR_NAME"
            fi
        fi
    fi

    if test -z "${author_email}"
    then
        author_email="${EMAIL:-}"

        if test -z "${author_email}"
        then
            author_email="$(git config --default '' --get author.email)"

            if test -z "${author_email}"
            then
                author_email="$(git config --default '' --get user.email)"

                if test -z "${author_email}"
                then
                    die "We can't get the author email from git, please fills `
                        `your git config file or use one of the env variables: `
                        `GIT_AUTHOR_EMAIL or EMAIL"
                fi
            fi
        fi
    fi

    if ! test_author_name_validity "${author_name}"
    then
        die "Bad author name '${author_name}'. Fix your git config file."
    elif ! test_author_email_validity "${author_email}"
    then
        die "Bad author email '${author_email}'. Fix your git config file."
    fi

    echo "${author_name} <${author_email}>"
}

test_license_presence() {
    if ! test -e "${ACT_LICENSE_FILE_PATH}"
    then
        die "The ACT license file is not stored in the expected place, add it `
            `in your repository here: ${ACT_LICENSE_FILE_PATH}"
    fi
}

# Tells if $1 is covered by dep5 (quick and dirty)
# Succeed if covered, fails if not covered.
test_path_in_dep5() {
    local -r path_to_test="$1"
    local -r dep5_file="${REPO_DIR}/.reuse/dep5"
    local covered="n"

    if test -f "${dep5_file}"
    then
        local dep5_covered_sub_pathes
        dep5_covered_sub_pathes="$(grep '^Files: ' "${dep5_file}" \
                                   | sed 's/^Files: //' \
                                   | sed 's/\*//')"


        for dep5_covered_sub_path in ${dep5_covered_sub_pathes}
        do
            if echo "${path_to_test}" | grep -qF "${dep5_covered_sub_path}"
            then
                covered="y"
                break;
            fi
        done
    fi

    test "${covered}" = "y"
}

run_reuse_init() {
    echo "Initializing copyrights from repo history..."

    test_license_presence

    local copyright
    copyright="$(get_copyright_info)"
    local -r copyright

    local processed_files_count="$((0))"

    # Reparse files from their git history
    export IFS="${NL}"
    for git_file in $(git ls-files "${OPT_TARGET}")
    do
        if test_path_in_dep5 "${git_file}"
        then
            echo "Skipping ${git_file} (covered by dep5)"
            continue
        fi

        # Note: "uniq|sort|uniq" is an optimisation for "sort|uniq"
        # when many consecutive items are identical
        local authors_name_email_year
        authors_name_email_year="$(\
            git log --follow --format="%aN;%aE;%ad" --date="format:%Y" \
                "${git_file}" \
            | uniq | sort | uniq)"

        for name_email_year in ${authors_name_email_year}
        do
            local author_name
            local author_email
            local author_year

            author_name="${name_email_year%%;*}"
            author_email="$(printf "%s" "${name_email_year}" | cut -d ';' -f 2)"
            author_year="${name_email_year##*;}"

            if ! test_author_name_validity "${author_name}"
            then
                die "Bad author name '${author_name}' for '${author_email}'. `
                    `Need a 'Good Name <${author_email}>' row in your .mailmap"
            elif ! test_author_email_validity "${author_email}"
            then
                die "Bad author email '${author_email}' for '${author_name}'. `
                    `Need a '<good@corp.email> <${author_email}>' row `
                    `in your .mailmap"
            fi

            # call reuse
            reuse annotate \
                --merge-copyrights \
                --license "${ACT_LICENSE_NAME}" \
                --copyright "${author_name} <${author_email}>" \
                --year "${author_year}" \
                "${git_file}" || \
                \
                (
                echo "Reuse issue detected. `
                     `Falling back to a dotlicense for ${git_file}"
                # Fallback to a forced dotlicence.
                # May be useful for two cases:
                # - if previous call did not detect filetype
                #    (waiting a future upstream --fallback-dot-license)
                # - if previous call crashed (fails to parse input file)
                reuse annotate \
                    --force-dot-license \
                    --merge-copyrights\
                    --license "${ACT_LICENSE_NAME}" \
                    --copyright "${author_name} <${author_email}>" \
                    --year "${author_year}" \
                    "${git_file}" || \
                    \
                    die "reuse crashed when working onto '${git_file}' `
                        `with author '${author_name} <${author_email}>' `
                        `(c) ${author_year}"
                )
        done

        processed_files_count="$((processed_files_count + 1))"
    done

    if test "${processed_files_count}" -eq 0
    then
        die "No files found matching filter '${OPT_TARGET}'"
    fi

    echo "${processed_files_count} files processed"
}

run_reuse_update() {
    echo "Updating copyrights of staged files..."

    test_license_presence

    local copyright
    copyright="$(get_copyright_info)"
    local -r copyright

    local processed_files_count="$((0))"

    # Process staged files
    export IFS="${NL}"
    for git_file in $(git diff --name-only --cached --diff-filter=d -- \
                      "${OPT_TARGET}")
    do
        if test_path_in_dep5 "${git_file}"
        then
            echo "Skipping ${git_file} (covered by dep5)"
            continue
        fi

        reuse annotate \
            --merge-copyrights \
            --license "${ACT_LICENSE_NAME}" \
            --copyright "${copyright}" \
            "${git_file}" || \
            \
            die "reuse crashed when working onto '${git_file}' `
                `with copyright '${copyright}'"

        processed_files_count="$((processed_files_count + 1))"
    done

    if test "${processed_files_count}" -eq 0
    then
        die "No staged files matching filter '${OPT_TARGET:-.}'"
    fi

    echo "${processed_files_count} files processed"
}

# Parse command line options and fire
main() {
    # Kind of enum
    local CMD=""
    local -r _CMD_HELP="CMD_HELP"
    local -r _CMD_HELP_FULL="CMD_HELP_FULL"
    local -r _CMD_INIT="CMD_INIT"
    local -r _CMD_UPDATE="CMD_UPDATE"

    # Parse global options, commands and command options in the same loop
    while test $# -gt 0
    do
        local OPT="$1"
        case "${OPT}" in
        help|--help|-h)
            CMD="${CMD}${_CMD_HELP}"
            shift
            ;;
        help-full|--help-full|-hh)
            CMD="${CMD}${_CMD_HELP_FULL}"
            shift
            ;;
        init)
            CMD="${CMD}${_CMD_INIT}"
            shift
            ;;
        update)
            CMD="${CMD}${_CMD_UPDATE}"
            shift
            ;;
        -*)
            die "Unknown option ${OPT}"
            ;;
        *)
            OPT_TARGET="${OPT}"
            shift
            ;;
        esac
    done

    # Execute command
    case "${CMD}" in
    "${_CMD_HELP}")
        print_help
        exit 0
        ;;
    "${_CMD_HELP_FULL}")
        print_help 3
        exit 0
        ;;
    "${_CMD_INIT}")
        run_reuse_init
        ;;
    "${_CMD_UPDATE}")
        run_reuse_update
        ;;
    "")
        # Update command is fired by default
        run_reuse_update
        ;;
    *)
        die "Only one command can be given, see --help"
        ;;
    esac
}

main "$@"
