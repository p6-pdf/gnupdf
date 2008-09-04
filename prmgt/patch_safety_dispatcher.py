# patch-safety-dispatcher.py
#
# This script runs other QA scripts like the test suite and a syntax check
# conforming to GCS.
# This script is intended to be imported by 'bzr' and executed before
# a commit is done. ('pre_commit' time in bzr terms)
###

# Copyright (C) 2008 Free Software Foundation, Inc

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.


from bzrlib import branch
from bzrlib import errors
import os

script_cmds = ['make syntax-check', 'make check']

def pre_commit_hook(local, master, old_revno, old_revid,
                   future_revno, future_revid, tree_delta, future_tree):
   success, fails = ([],[])
   for c in script_cmds:
       if (os.system(c) != 0):
           fails.append(c)
       else:
           success.append(c)
   print "\n### Patch safety report ###"
   for s in success:
       print ">>> " + s + ": Succeeded"
   for f in fails:
       print ">>> " + f + ": Failed !!"
   if len(f) > 0:
       raise errors.BzrError("One or more scripts failed, fix the patch,"
                             " lazy!!\nNote: commit is not applied.")
   else:
       print "All tests succeeded, patch is safe.\n"

branch.Branch.hooks.install_named_hook('pre_commit', pre_commit_hook,
                                'Patch safety scripts hook')
