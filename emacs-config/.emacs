;; .emacs

;;; uncomment this line to disable loading of "default.el" at startup
;; (setq inhibit-default-init t)

;; turn on font-lock mode
(when (fboundp 'global-font-lock-mode)
  (global-font-lock-mode t))

;; enable visual feedback on selections
;(setq transient-mark-mode t)

;; default to better frame titles
(setq frame-title-format
      (concat  "%b - emacs@" (system-name)))

;; default to unified diffs
(setq diff-switches "-u")

;; always end a file with a newline
;(setq require-final-newline 'query)

(setq-default make-backup-files nil)
(define-key ctl-x-map "l" 'goto-line)
(define-key ctl-x-map "r" 'replace-string)
(setq column-number-mode t)

;; set up unicode
(prefer-coding-system       'utf-8)
(set-default-coding-systems 'utf-8)
(set-terminal-coding-system 'utf-8)
(set-keyboard-coding-system 'utf-8)
;; This from a japanese individual.  I hope it works.
(setq default-buffer-file-coding-system 'utf-8)
;; From Emacs wiki
(setq x-select-request-type '(UTF8_STRING COMPOUND_TEXT TEXT STRING))

(add-to-list 'load-path "/root/.emacs.d/auto-complete")
(require 'auto-complete-config)
(add-to-list 'ac-dictionary-directories "/root/.emacs.d/auto-complete/ac-dict")
(ac-config-default)

(add-to-list 'load-path "~/.emacs.d/auto-complete-clang")

(require 'auto-complete-clang)
(setq ac-auto-start t)
(setq ac-quick-help-delay 0.5)
;; (ac-set-trigger-key "TAB")
;; (define-key ac-mode-map  [(control tab)] 'auto-complete)
(define-key ac-mode-map  [(control tab)] 'auto-complete)
(defun my-ac-config ()
  (setq ac-clang-flags
        (mapcar(lambda (item)(concat "-I" item))
               (split-string
                "
 /usr/lib/gcc/armv6l-unknown-linux-gnueabihf/4.9.2/../../../../include/c++/4.9.2
 /usr/lib/gcc/armv6l-unknown-linux-gnueabihf/4.9.2/../../../../include/c++/4.9.2/armv6l-unknown-linux-gnueabihf
 /usr/lib/gcc/armv6l-unknown-linux-gnueabihf/4.9.2/../../../../include/c++/4.9.2/backward
 /usr/lib/gcc/armv6l-unknown-linux-gnueabihf/4.9.2/include
 /usr/local/include
 /usr/lib/gcc/armv6l-unknown-linux-gnueabihf/4.9.2/include-fixed
 /usr/include
")))
  (setq-default ac-sources '(ac-source-abbrev ac-source-dictionary ac-source-words-in-same-mode-buffers))
  (add-hook 'emacs-lisp-mode-hook 'ac-emacs-lisp-mode-setup)
  ;; (add-hook 'c-mode-common-hook 'ac-cc-mode-setup)
  (add-hook 'ruby-mode-hook 'ac-ruby-mode-setup)
  (add-hook 'css-mode-hook 'ac-css-mode-setup)
  (add-hook 'auto-complete-mode-hook 'ac-common-setup)
  (global-auto-complete-mode t))
(defun my-ac-cc-mode-setup ()
  (setq ac-sources (append '(ac-source-clang ac-source-yasnippet) ac-sources)))
(add-hook 'c-mode-common-hook 'my-ac-cc-mode-setup)
;; ac-source-gtags
(my-ac-config)

;; format code
(defun indent-buffer ()
"Indent the whole buffer."
(interactive)
(indent-region (point-min) (point-max) nil))
(global-set-key [f7] 'indent-buffer)
