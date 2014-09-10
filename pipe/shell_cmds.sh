""" Commonly used Filters
    cat:    Copies input direct to output
    head:   Shows beginning of a file (default 10 lines)
    tail:   Shows end of a file (default 10 lines)
    wc:	    Counts lines, words and characters
    sort:   Sorts input lines
    grep:   Shows lines that match a regular expression
    tr:	    Translates or deletes speciified character sets
    sed:    Stream editor
    uniq:   Discards all but one of successive identical lines
    awk:    Highly-programmable field-processing
"""




# Converts strings in foo.txt from upper-case to lower-case
sort foo.txt | tr '[A-Z]' '[a-z]' > sorted.txt

# Semi-filters
# will count number of processes running on machine
ps aex |wc
# will show the symbolic links in current directory
ls -l | grep '^l'
# browsing program less, printing utility lpr, command-line mailer mail.
# such programs can be used at downstream end of a pipeline, for example:
grep syslog /var/log/messages | less

# sed - the stream editor
# supportsautomated editing of text and more flexible filter

