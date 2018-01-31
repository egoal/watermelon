(defun make-cd(title artist rating ripped)
    (list :title title :artist artist :rating rating :ripped ripped
    )
)

;; global value
(defvar *db* nil)

(defun add-record(cd) (push cd *db*))

(add-record(make-cd "Rose" "Kathy Mattea" 7 t))
(add-record(make-cd "Fly" "Dixie Chicks" 8 t))
(add-record(make-cd "Home" "Dixie Chicks" 9 t))

(defun dump-db()
    (dolist (cd *db*)
        (format t "~{~a:~10t~a~%~}~%" cd
        )
    )
)

(defun prompt-read(prompt)
    (format *query-io* "~a: " prompt)
    (force-output *query-io*)
    (read-line *query-io*)
)

(defun prompt-for-cd()
    (make-cd 
        (prompt-read "Title")
        (prompt-read "Artist")
        (or (parse-integer (prompt-read "Rating") :junk-allowed t) 0)
        (y-or-n-p "Ripped")
    )
)

(defun add-cds()
    (loop (add-record (prompt-for-cd))
        (if (not (y-or-n-p "Another?:")) (return)
        )
    )
)

(defun save-db(filename)
    (with-open-file (out filename :direction :output :if-exists :supersede)
        (with-standard-io-syntax (print *db* out))
    )
)

(defun load-db(filename)
    (with-open-file (in filename)
        (with-standard-io-syntax (setf *db* (read in)))
    )
)

(defun select-by-artist(artist)
    (remove-if-not 
        #'(lambda (cd)(equal (getf cd :artist) artist) ) 
        *db*
    )
)

(defun select(selector-fn)
    (remove-if-not selector-fn *db*)
)

; #' get the function with the name
; (select #'(lambda (cd)(equal (getf cd :artist) "Dixie Chicks") ))

(defun artist-selector(artist) #'(lambda (cd) (equal (getf cd :artist) artist)))

; (selecr (artist-selector "DixChicks"))

;; default value
(defun foo(&key a (b 10) (c 20 c-p))
    (list a b c c-p)
)

; (foo :a 0 :b 1 :c 2)  ->(0 1 2 T)

(defun where(&key title artist rating (ripped nil ripped-p))
    #'(lambda (cd) 
        (and 
            (if title (equal (getf cd :title) title) t)
            (if artist (equal (getf cd :artist) artist) t)
            (if rating (equal (getf cd :rating) rating) t)
            (if ripped-p (equal (getf cd :ripped) ripped) t)
        )
    )
)

; (select (where :rating 8))

(defun update(selector-fn &key title artist rating (ripped nil ripped-p))
    (setf *db*
        (mapcar 
            #'(lambda (row)
                (when (funcall selector-fn row)
                    (if title (equal (getf cd :title) title) t)
                    (if artist (equal (getf cd :artist) artist) t)
                    (if rating (equal (getf cd :rating) rating) t)
                    (if ripped-p (equal (getf cd :ripped) ripped) t)
                )
                *db*
            )
        )
    )
)

; (update (where :artist "Dixie Chicks") :rating 11)

(defun delete-rows(selector-fn)
    (setf *db* (remove-if selector-fn *db*))
)


