# philex package
# Matthew Bertucci 2022/08/18 for v1.3

#include:xspace
#include:calc
#include:cgloss4e
#include:linguex
#include:ifthen
#include:suffix

#keyvals:\usepackage/philex#c
oldpunct
hyper
draft
#endkeyvals

#ifOption:oldpunct
\km
\kmt
\p
\pt
\q
\qt
\s
\stp
#endif

\bpaformat{numstyle}{prefix}{suffix}
\bpbformat{numstyle}{prefix}{suffix}
\bpcformat{numstyle}{prefix}{suffix}
\bpdformat{numstyle}{prefix}{suffix}
\broff
\bron
\lb[c]{label}{content%text}#l
\lb{label}{content%text}#l
\lba[labeltext]{label}{content%text}#l
\lba{label}{content%text}#l
\lbb[labeltext]{label}{content%text}#l
\lbb{label}{content%text}#l
\lbp*[c]{label}{labeltext}{content%text}#l
\lbp*{label}{labeltext}{content%text}#l
\lbp[c]{label}{labeltext}{content%text}#l
\lbp{label}{labeltext}{content%text}#l
\lbpa*[c]{label}{content}#l
\lbpa*{label}{content}#l
\lbpa[c]{label}{content}#l
\lbpa{label}{content}#l
\lbpb*[c]{label}{content}#l
\lbpb*{label}{content}#l
\lbpb[c]{label}{content}#l
\lbpb{label}{content}#l
\lbpc*[c]{label}{content}#l
\lbpc*{label}{content}#l
\lbpc[c]{label}{content}#l
\lbpc{label}{content}#l
\lbpd*[c]{label}{content}#l
\lbpd*{label}{content}#l
\lbpd[c]{label}{content}#l
\lbpd{label}{content}#l
\lbpsep[c]{label}{reflabel%ref}{labeltext}{content%text}#l
\lbpsep{label}{reflabel%ref}{labeltext}{content%text}#l
\lbu*[c]{label}{reflabel%ref}{suffix}{content%text}#l
\lbu*{label}{reflabel%ref}{suffix}{content%text}#l
\lbu[c]{label}{reflabel%ref}{suffix}{content%text}#l
\lbu{label}{reflabel%ref}{suffix}{content%text}#l
\lbusep[c]{label}{reflabel%ref}{suffix}{envlabel%ref}{content%text}#l
\lbusep{label}{reflabel%ref}{suffix}{envlabel%ref}{content%text}#l
\lbz[labeltext]{label}{content%text}#l
\lbz{label}{content%text}#l
\ml{label}#r
\narrowcenter
\oddity{text%plain}
\philbrackets{delim1}{delim2}
\philcomma
\phildashes{delim1}{delim2}
\philempty
\philexclaim
\philfullstop
\philpunct
\philquestion
\philsubcomma
\philsubempty
\philsubexclaim
\philsubpunct
\philsubquestion
\philsubstop
\phlabelsep{length}
\phlabelsepdefault
\rf[suffix]{label}#r
\rf{label}#r
\rff[suffix]{label}#r
\rff{label}#r
\rffnot[suffix]{label}#r
\rffnot{label}#r
\rfx[suffix]{label}#r
\rfx{label}#r
\rn[suffix]{label}#r
\rn{label}#r
\rnx[suffix]{label}#r
\rnx{label}#r
\rp{label}#r
\sepset{label}{
\subformat{numstyle}{prefix}{suffix}
\subsubformat{numstyle}{prefix}{suffix}
\widecenter

# not documented
\bn#*
\bns#*
\bota{counter}#S
\botb{counter}#S
\botc{counter}#S
\botd{counter}#S
\bpasize#*
\bpbsize#*
\bpcsize#*
\bpdsize#*
\centro#*
\firstphildash#*
\grlen#*
\hyperreffalse#S
\hyperreftrue#S
\ifhyperref#S
\ifoldpunct#S
\ifphildraft#S
\lbpaStar[c]{label}{content}#Sl
\lbpaStar{label}{content}#Sl
\lbpbStar[c]{label}{content}#Sl
\lbpbStar{label}{content}#Sl
\lbpcStar[c]{label}{content}#Sl
\lbpcStar{label}{content}#Sl
\lbpdStar[c]{label}{content}#Sl
\lbpdStar{label}{content}#Sl
\lbpStar[c]{label}{labeltext}{content%text}#Sl
\lbpStar{label}{labeltext}{content%text}#Sl
\lbuStar[c]{label}{reflabel%ref}{suffix}{content%text}#Sl
\lbuStar{label}{reflabel%ref}{suffix}{content%text}#Sl
\lebrack#*
\ncentro#*
\oldpunctfalse#S
\oldpuncttrue#S
\phildraftfalse#S
\phildrafttrue#S
\philmarginfactor#*
\philsemi#*
\rfp{label}#*r
\ribrack#*
\rsep{label}#*r
\sa#*
\sab#*
\seba#*
\sebatemp#S
\secondphildash#*
\sr#*
\subettan{counter}#S
\tempa#S
\tempb#S
\tempc#S
\thealtsub#*
\thealtsubsub#*
\thebna#*
\thebpa#*
\thebpb#*
\thebpc#*
\thebpd#*
\wcentro#*