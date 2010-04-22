;;; authors.el --- utility for maintaining GNU PDF' AUTHORS file -*-coding: utf-8;-*-

;; Copyright (C) 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008,
;;   2009  Free Software Foundation, Inc.

;; Author: Gerd Moellmann <gerd@gnu.org>
;; Maintainer: GNU PDF Dev Team <pdf-devel@gnu.org>
;; Keywords: maint

;; This file is NOT part of GNU Emacs.

;; GNU Emacs is free software: you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation, either version 3 of the License, or
;; (at your option) any later version.

;; GNU Emacs is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.

;;; Commentary:

;; Use M-x authors RET to create an *Authors* buffer that can used as
;; or merged with libgnupdf' AUTHORS file.

;;; Code:

(defvar authors-coding-system 'utf-8
  "Coding system used in the AUTHORS file.")

(defconst authors-many-files 20
  "Maximum number of files for which to print individual information.
If an author has modified more files, only the names of the most
frequently modified files are printed and a count of the additional
files.")

(defconst authors-aliases
  '(("Aleksander Morgado" "Aleksander Morgado Juez"))
  "Alist of author aliases.

Each entry is of the form (REALNAME REGEXP...).  If an author's name
matches one of the REGEXPs, use REALNAME instead.
If REALNAME is nil, ignore that author.")

(defconst authors-fixed-case
  nil
  "List of authors whose names cannot be simply capitalized.")

(defvar authors-public-domain-files
  nil
  "List of regexps matching files for which the FSF doesn't need papers.")


(defvar authors-obsolete-files-regexps
  nil
  "List of regexps matching obsolete files.
Changes to files matching one of the regexps in this list are not
listed.")

(defconst authors-ignored-files
  '("NEWS" "ORDERS" "PROBLEMS" "FAQ" "AUTHORS" "FOR-RELEASE" "TODO" "todo"
    "Makefile.am"
    "install-sh" "missing" "mkinstalldirs"
    "doc/Makefile.in" "prmgt/Makefile.in"
    "lib/Makefile.in" "utils/Makefile.in"
    "torture/unit/Makefile.in" "torture/Makefile.in"
    "torture/testdata/Makefile.in" "Makefile.in"
    "build-aux/Makefile.in" "src/Makefile.in"
    "doc/Makefile.am" "prmgt/Makefile.am"
    "lib/Makefile.am" "utils/Makefile.am"
    "torture/Makefile.am" "torture/unit/Makefile.am"
    "torture/testdata/Makefile.am" "build-aux/Makefile.am"
    "src/Makefile.am"
    "lib" "m4")
  "List of files and directories to ignore.
Changes to files in this list are not listed.")

;; FIXME :cowrote entries here can be overwritten by :wrote entries
;; derived from a file's Author: header (eg mh-e).  This really means
;; the Author: header is erroneous.
(defconst authors-fixed-entries
  '(("Hardy Falk" :changed "src/base/pdf-fp-func.c"
     "src/base/pdf-fp-func.h"))
  "Actions taken from the original, manually (un)maintained AUTHORS file.")


(defconst authors-valid-file-names
  '("m4/ulonglong.m4"
    "prmgt/libgnupdf.tjp"
    "torture/generate_testall"
    "prmgt/pmccabe2html"
    "prmgt/pmccabe.css"
    "test-rectangle"
    "m4/onceonly_2_57.m4"
    "doc/figures/stream-value-internals.txt"
    "doc/figures/object-document-architecture.txt"
    "doc/figures/dctdec-nfa.eps"
    "doc/figures/object-pool.txt"
    "doc/figures/object-layer-architecture.txt"
    "doc/figures/dctdec-nfa.fig"
    "doc/figures/dctdec-nfa.jpg"
    "doc/figures/dctdec-nfa.png"
    "prmgt/check-api-doc-consistency.pl"
    "prmgt/pertdot.xsl"
    "prmgt/pertfdp.xsl")
  "File names which are valid, but no longer exist (or cannot be
found) in the repository.")

(defconst authors-renamed-files-alist
  '(;; Moved to different directories.
    ("prmgt/pmccabe2html.in" . "build-aux/pmccabe2html")
    )
  "Alist of files which have been renamed during their lifetime.
Elements are (OLDNAME . NEWNAME).")

(defconst authors-renamed-files-regexps
  '(("^m/m-\\(.*\\.h\\)$" . "m/\\1")
    ("^m-\\(.*\\.h\\)$" . "\\1")
    ("^s/s-\\(.*\\.h\\)$" . "s/\\1")
    ("^s-\\(.*\\.h\\)$" . "\\1")
    ("^s/[-.a-zA-Z0-9_]+\\.h$" . t)
    ("\\(.*\\)\\.cmd$" . "\\1.bat")
    ("\\.bat$" . t)
    ("\\.[ch]$" . t)
    ("\\.el$" . t)
    ("\\.ps$" . t)
    ("\\.texi?$" . t)
    ("\\.texinfo$" . t)
    ("\\.xml?$" . t)
    ("\\.x[pb]m$" . t)
    ("\\.[xp]bm$" . t)
    ("^paths\\." . t)
    ("^install\\." . t)
    ("^\\(TUTORIAL[^/]*\\)" . "tutorials/\\1")
    ("^\\(tree-widget/\\(?:default\\|folder\\)/[-a-z]+\\.png\\)$" .
     "images/\\1")
    ("^\\(images/icons/\\)mac\\(emacs\\)_\\([0-9]+\\)\\(\\.png\\)" .
     "\\1\\2\\3_mac\\4")
    ("\\(images/icons/\\)emacs_\\([0-9][0-9]\\)\\.png" .
     "\\1hicolor/\\2x\\2/apps/emacs.png")
    )
  "List regexps and rewriting rules for renamed files.
Elements are (REGEXP . REPLACE).  If REPLACE is a string, the file
name matching REGEXP is replaced by REPLACE using `replace-string'.
Otherwise, the file name is accepted as is.")

(defvar authors-checked-files-alist)
(defvar authors-invalid-file-names)

(defun authors-canonical-file-name (file log-file pos author)
  "Return canonical file name for FILE found in LOG-FILE.
Checks whether FILE is a valid (existing) file name, has been renamed,
or is on the list of removed files.  Returns the non-diretory part of
the file name.  Only uses the LOG-FILE position POS and associated AUTHOR
to print a message if FILE is not found."
  ;; FILE should be re-checked in every different directory associated
  ;; with a LOG-FILE.  Eg configure.in from src/ChangeLog is not the
  ;; same as that from top-level/ChangeLog.
  (let* ((fullname (expand-file-name file (file-name-directory log-file)))
	 (entry (assoc fullname authors-checked-files-alist))
	 relname
	 valid)
    (if entry
	(cdr entry)
      (setq relname (file-name-nondirectory file))
      (if (or (member relname authors-valid-file-names)
	      (file-exists-p file)
	      (file-exists-p relname)
	      (file-exists-p (concat "etc/" relname)))
	  (setq valid relname)
	(setq valid (assoc file authors-renamed-files-alist))
	(if valid
	    (setq valid (cdr valid))
	  (let ((rules authors-renamed-files-regexps))
	    (while rules
	      (if (string-match (car (car rules)) file)
		  (setq valid (if (stringp (cdr (car rules)))
				  (file-name-nondirectory
				   (replace-match (cdr (car rules)) t nil file))
				relname)
			rules nil))
	      (setq rules (cdr rules))))))
      (setq authors-checked-files-alist
	    (cons (cons fullname valid) authors-checked-files-alist))
      (unless (or valid
		  (member file authors-ignored-files)
		  (string-match "[*]" file)
		  (string-match "^[0-9.]+$" file))
	(setq authors-invalid-file-names
	      (cons (format "%s:%d: unrecognized `%s' for %s"
			    log-file
			    (1+ (count-lines (point-min) pos))
			    file author)
		    authors-invalid-file-names)))
      valid)))

(defun authors-add-fixed-entries (table)
  "Add actions from `authors-fixed-entries' to TABLE."
  (dolist (entry authors-fixed-entries)
    (let ((author (car entry))
	  action)
      (dolist (item (cdr entry))
	(if (symbolp item)
	    (setq action item)
	  (authors-add author item action table))))))


(defun authors-obsolete-file-p (file)
  "Return non-nil if FILE is obsolete.
FILE is considered obsolete if it matches one of the regular expressions
from `authors-obsolete-files-regexps'."
  (let (obsolete-p
	(regexps authors-obsolete-files-regexps))
    (while (and regexps (not obsolete-p))
      (setq obsolete-p (string-match (car regexps) file)
	    regexps (cdr regexps)))
    obsolete-p))


(defun authors-add (author file action table)
  "Record that AUTHOR worked on FILE.
ACTION is a keyword symbol describing what he did.  Record file,
author and what he did in hash table TABLE.  See the description of
`authors-scan-change-log' for the structure of the hash table."
  (unless (or (member file authors-ignored-files)
	      (authors-obsolete-file-p file)
	      (equal author ""))
    (let* ((value (gethash author table))
	   (entry (assoc file value))
	   slot)
      (if (null entry)
	  (puthash author (cons (list file (cons action 1)) value) table)
	(if (setq slot (assoc action (cdr entry)))
	    (setcdr slot (1+ (cdr slot)))
	  (nconc entry (list (cons action 1))))))))


(defun authors-canonical-author-name (author)
  "Return a canonicalized form of AUTHOR, an author name.
If AUTHOR has an entry in `authors-aliases', use that.  Remove
email addresses.  Capitalize words in the author's name, unless
it is found in `authors-fixed-case'."
  (let* ((aliases authors-aliases)
	 regexps realname)
    (while aliases
      (setq realname (car (car aliases))
	    regexps (cdr (car aliases))
	    aliases (cdr aliases))
      (while regexps
	(if (string-match (car regexps) author)
	    (setq author realname
		  regexps nil
		  aliases nil)
	  (setq regexps (cdr regexps))))))
  (when author
    (setq author (replace-regexp-in-string "[ \t]*[(<].*$" "" author))
    (setq author (replace-regexp-in-string "\`[ \t]+" "" author))
    (setq author (replace-regexp-in-string "[ \t]+$" "" author))
    (setq author (replace-regexp-in-string "[ \t]+" " " author))
    (unless (string-match "[-, \t]" author)
      (setq author ""))
    (or (car (member author authors-fixed-case))
	(capitalize author))))

(defun authors-scan-change-log (log-file table)
  "Scan change log LOG-FILE for author information.

For each change mentioned in the log, add an entry to hash table TABLE
under the author's canonical name.

Keys of TABLE are author names.  Values are alists of entries (FILE
\(ACTION . COUNT) ...).  FILE is one file the author worked on.  The
rest of the entry is a list of keyword symbols describing what he did
with the file and the number of each action:

:wrote		means the author wrote the file
:cowrote	means he wrote the file in collaboration with others
:changed	means he changed the file COUNT times."

  (let* ((enable-local-variables :safe)	; for find-file, hence let*
	 (enable-local-eval nil)
	 (existing-buffer (get-file-buffer log-file))
	 (buffer (find-file-noselect log-file))
	 authors file pos)
    (with-current-buffer buffer
      (save-restriction
	(widen)
	(goto-char (point-min))
	(while (re-search-forward "^[0-9]\\|^[ \t]+\\* " nil t)
	  (beginning-of-line)
	  (setq pos (point))
	  (cond ((looking-at "^[0-9]+-[0-9]+-[0-9]+")
		 ;; Handle joint authorship of changes.
		 ;; This can be a bit fragile, and is not too common.
		 (setq authors nil)
		 (while (progn
			  (skip-chars-forward " \t+:0-9-")
			  (not (looking-at "\\($\\|\\*\\|\
Suggested\\|Trivial\\|Version\\|Originally\\|From:\\|Patch[ \t]+[Bb]y\\)")))
		   (push (authors-canonical-author-name
			  (buffer-substring-no-properties
			   (point) (line-end-position))) authors)
		   (forward-line 1)))
		((looking-at "^[ \t]+\\*")
		 (let ((line (buffer-substring-no-properties
			      (match-end 0) (line-end-position))))
		   (while (and (not (string-match ":" line))
			       (forward-line 1)
			       (not (looking-at ":\\|^[ \t]*$")))
		     (setq line (concat line
					(buffer-substring-no-properties
					 (line-beginning-position)
					 (line-end-position)))))
		   (when (string-match ":" line)
		     (setq line (substring line 0 (match-beginning 0)))
		     (setq line (replace-regexp-in-string "[[(<{].*$" "" line))
		     (setq line (replace-regexp-in-string "," "" line))
		     (dolist (file (split-string line))
		       (when (setq file (authors-canonical-file-name file log-file pos (car authors)))
			 (dolist (author authors)
			   ;;(message "%s changed %s" author file)
			   (authors-add author file :changed table)))))
		   (forward-line 1)))))))
    (unless existing-buffer
      (kill-buffer buffer))))


(defun authors-scan-el (file table)
  "Scan Lisp file FILE for author information.
TABLE is a hash table to add author information to."
  (let* ((existing-buffer (get-file-buffer file))
	 (enable-local-variables :safe)	; for find-file, hence let*
	 (enable-local-eval nil)
	 (buffer (find-file-noselect file)))
    (setq file (file-name-nondirectory file))
    (with-current-buffer buffer
      (save-restriction
	(widen)
	(goto-char (point-min))
	(while (and (re-search-forward
		     "^;+[ \t]*\\(Authors?\\|Commentary\\|Code\\):[ \t]*" nil t)
		    (not (member (match-string 1) '("Commentary" "Code"))))
	  (let ((continue t)
		(action :wrote)
		authors)
	    (while continue
	      ;; Some entries contain a year range in front of the
	      ;; author's name.
	      (skip-chars-forward "-0-9 \t")
	      (push (authors-canonical-author-name
		     (buffer-substring-no-properties
		      (point) (line-end-position))) authors)
	      ;; tips.texi says the continuation line should begin
	      ;; with a tab, but often spaces are used.
	      (setq continue
		    (and (zerop (forward-line 1))
			 (looking-at ";;;?\\(\t+ *\\|  +\\)[[:alnum:]]")
			 (goto-char (1- (match-end 0)))
			 (not (looking-at "[[:upper:]][-[:alpha:]]+:[ \t]")))))
	    (and (> (length authors) 1)
		 (setq action :cowrote))
	    (mapc (lambda (author)
		    (authors-add author file action table))
		  authors)))))
    (unless existing-buffer
      (kill-buffer buffer))))


(defun authors-public-domain-p (file)
  "Return t if FILE is a file that was put in public domain."
  (let ((public-domain-p nil)
	(list authors-public-domain-files))
    (while (and list (not public-domain-p))
      (when (string-match (car list) file)
	(setq public-domain-p t))
      (setq list (cdr list)))
    public-domain-p))

(defvar authors-author-list)

(defun authors-add-to-author-list (author changes)
  "Insert information about AUTHOR's work on libgnupdf into `authors-author-list'.
CHANGES is an alist of entries (FILE (ACTION . COUNT) ...), as produced by
`authors-scan-change-log'.
The element added to `authors-author-list' is (AUTHOR WROTE CO-WROTE CHANGED),
where WROTE, CO-WROTE, and CHANGED are lists of the files written, co-written
and changed by AUTHOR."
  (when author
    (let ((nchanged 0)
	  wrote-list
	  cowrote-list
	  changed-list)
      (dolist (change changes)
	(let* ((actions (cdr change))
	       (file (car change))
	       (filestat (if (authors-public-domain-p file)
			     (concat file " (public domain)")
			   file))
	       slot)
	  (cond ((assq :wrote actions)
		 (setq wrote-list (cons filestat wrote-list)))
		((assq :cowrote actions)
		 (setq cowrote-list (cons filestat cowrote-list)))
		(t
		 (setq changed-list
		       (cons (cons file (cdr (assq :changed actions)))
			     changed-list))))))
      (if wrote-list
	  (setq wrote-list (sort wrote-list 'string-lessp)))
      (if cowrote-list
	  (setq cowrote-list (sort cowrote-list 'string-lessp)))
      (when changed-list
	(setq changed-list (sort changed-list
				 (lambda (a b)
				   (if (= (cdr a) (cdr b))
				       (string-lessp (car a) (car b))
				     (> (cdr a) (cdr b))))))
	(setq nchanged (length changed-list))
	(setq changed-list (mapcar 'car changed-list)))
      (if (> (- nchanged authors-many-files) 2)
	  (setcdr (nthcdr authors-many-files changed-list)
		  (list (format "and %d other files" (- nchanged authors-many-files)))))
      (setq authors-author-list
	    (cons (list author wrote-list cowrote-list changed-list)
		  authors-author-list)))))

(defun authors (root)
  "Extract author information from change logs and Lisp source files.
ROOT is the root directory under which to find the files.  If called
interactively, ROOT is read from the minibuffer.
Result is a buffer *Authors* containing authorship information, and a
buffer *Authors Errors* containing references to unknown files."
  (interactive "Dlibgnupdf source directory: ")
  (setq root (expand-file-name root))
  (let ((logs (process-lines find-program root "-name" "ChangeLog*"))
	(table (make-hash-table :test 'equal))
	(buffer-name "*Authors*")
	authors-checked-files-alist
	authors-invalid-file-names)
    (authors-add-fixed-entries table)
    (unless (file-exists-p (expand-file-name "src/base/pdf-types.h" root))
      (unless (y-or-n-p
	       (format "Not the root directory of libgnupdf: %s, continue? " root))
	(error "Not the root directory")))
    (dolist (log logs)
      (when (string-match "ChangeLog\\(.[0-9]+\\)?$" log)
	(message "Scanning %s..." log)
	(authors-scan-change-log log table)))
    (let ((els (process-lines find-program root "-name" "*.el")))
      (dolist (file els)
	(message "Scanning %s..." file)
	(authors-scan-el file table)))
    (message "Generating buffer %s..." buffer-name)
    (set-buffer (get-buffer-create buffer-name))
    (erase-buffer)
    (set-buffer-file-coding-system authors-coding-system)
    (insert
"Many people have contributed code included in the Free Software
Foundation's distribution of the GNU PDF Library.  To show our
appreciation for their public spirit, we list here in
alphabetical order a condensed list of their contributions.\n")
    (let (authors-author-list a)
      (maphash #'authors-add-to-author-list table)
      (setq authors-author-list
	    (sort authors-author-list
		  (lambda (a b) (string-lessp (car a) (car b)))))
      (dolist (a authors-author-list)
	(let ((author (car a))
	      (wrote (nth 1 a))
	      (cowrote (nth 2 a))
	      (changed (nth 3 a))
	      file)
	(insert "\n" author ": ")
	(when wrote
	  (insert "wrote")
	  (dolist (file wrote)
	    (if (> (+ (current-column) (length file)) 72)
	      (insert "\n "))
	    (insert " " file))
	  (insert "\n"))
	(when cowrote
	  (if wrote
	      (insert "and "))
	  (insert "co-wrote")
	  (dolist (file cowrote)
	    (if (> (+ (current-column) (length file)) 72)
	      (insert "\n "))
	    (insert " " file))
	  (insert "\n"))
	(when changed
	  (if (or wrote cowrote)
	      (insert "and "))
	  (insert "changed")
	  (dolist (file changed)
	    (if (> (+ (current-column) (length file)) 72)
		(insert "\n "))
	    (insert " " file))
	  (insert "\n")))))
    (insert "\nLocal" " Variables:\ncoding: "
	    (symbol-name authors-coding-system) "\nEnd:\n")
    (message "Generating buffer %s... done" buffer-name)
    (unless noninteractive
      (when authors-invalid-file-names
	(with-current-buffer (get-buffer-create "*Authors Errors*")
	  (setq buffer-read-only nil)
	  (erase-buffer)
	  (set-buffer-file-coding-system authors-coding-system)
	  (insert "Unrecognized file entries found:\n\n")
	  (mapc (lambda (f) (if (not (string-match "^[A-Za-z]+$" f)) (insert f "\n")))
		(sort authors-invalid-file-names 'string-lessp))
	  (goto-char (point-min))
	  (compilation-mode)
	  (message "Errors were found.  See buffer %s" (buffer-name))))
      (pop-to-buffer buffer-name))))


(defun batch-update-authors ()
  "Produce an AUTHORS file.
Call this function in batch mode with two command line arguments FILE
and ROOT.  FILE is the file to write, ROOT is the root directory of
the libgnupdf source tree, from which to build the file."
  (unless noninteractive
    (error "`batch-update-authors' is to be used only with -batch"))
  (when (/= (length command-line-args-left) 2)
    (error "Call `batch-update-authors' with the name of the file to write"))
  (let* ((file (pop command-line-args-left))
	 (root (pop command-line-args-left)))
    (authors root)
    (write-file file)))

(provide 'authors)

;; arch-tag: 659d5900-5ff2-43b0-954c-a315cc1e4dc1
;;; authors.el ends here
