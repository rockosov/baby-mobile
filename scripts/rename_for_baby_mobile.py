import os
import re

dirname, dirnames, filenames = next(os.walk("."))

filecount = 1
filename_length = 4
mp3_pattern = re.compile("^.*\.mp3", re.DOTALL)
for old_filename in sorted(filenames):
    if mp3_pattern.match(old_filename):
        new_filename = str(filecount).zfill(filename_length) + ".mp3"
        print "Rename %s => %s" % (old_filename, new_filename)
        os.rename(old_filename, new_filename)
        filecount += 1

