#!/bin/bash

pandoc --from markdown --to latex -o $1.tex $1
sed -e 's/includegraphics/includegraphics\[width\=\\textwidth\]/g' $1.tex > body.tex
pdflatex template.tex
convert -density 169 template.pdf -quality 95 -colorspace sRGB -alpha remove $1.jpg
tprint $1.png
rm -f *.log *.aux *.pdf *.out *.md.tex