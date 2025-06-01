
#ifndef QT_NO_DEBUG
#include "mostQtHeaders.h"
#include "tablemanipulation_t.h"
#include "tablemanipulation.h"
#include "qdocumentcursor.h"
#include "qdocument.h"
#include "qeditor.h"
#include "latexdocument.h"
#include "testutil.h"
#include <QtTest/QtTest>
TableManipulationTest::TableManipulationTest(QEditor* editor): ed(editor){}

void TableManipulationTest::splitColDef_data(){
	QTest::addColumn<QString>("def");
	QTest::addColumn<QString>("splittedDef"); // for simplicity use a single string instead of a QStringList and '-' as separator

	QTest::newRow("one col") << "l" << "l";
	QTest::newRow("few cols") << "lrrc" << "l-r-r-c";
	QTest::newRow("hrows") << "l|rr|c" << "l-|r-r-|c";
	QTest::newRow("allHrows") << "|l|r|" << "|l-|r|";
	QTest::newRow("supportedChars") << "lcrsSp" << "l-c-r-s-S-p";
	QTest::newRow("atOperator") << "l@{someText}r" << "l-@{someText}r";
	QTest::newRow("repetion") << "*{3}{lr}" << "l-r-l-r-l-r";
    QTest::newRow("repetion2") << "l|*{3}{c}" << "l-|c-c-c";
	QTest::newRow("block") << "p{width}l" << "p{width}-l";
	QTest::newRow("option") << "rs[key=val]l" << "r-s[key=val]-l";
}

void TableManipulationTest::splitColDef(){
	QFETCH(QString, def);
	QFETCH(QString, splittedDef);

	QEQUAL(LatexTables::splitColDef(def).join("-"), splittedDef);
}

void TableManipulationTest::simplifyColDef_data(){
	// for simplicity use a single string instead of a QStringList and '-' as separator
	QTest::addColumn<QString>("def");
	QTest::addColumn<QString>("simplified");

	QTest::newRow("hline") << "|l-|r-r-|c|" << "l-r-r-c";
	QTest::newRow("atOperator") << "l-@{someText}r" << "l-r";
	QTest::newRow("block") << "p{width}-l" << "p-l";
	QTest::newRow("option") << "r-s[key=val]-l" << "r-s-l";
}

void TableManipulationTest::simplifyColDef(){
	QFETCH(QString, def);
	QFETCH(QString, simplified);

	QStringList res = def.split('-');
	LatexTables::simplifyColDefs(res);

	QEQUAL(res.join("-"), simplified);
}

void TableManipulationTest::addCol_data(){
	QTest::addColumn<QString>("text");
	QTest::addColumn<int>("row");
	QTest::addColumn<int>("col");
	QTest::addColumn<QString>("newText");
	
	//-------------cursor without selection--------------
    QTest::newRow("add first col")
		<< "\\begin{tabular}{xy}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 0
		<< "\\begin{tabular}{lxy}\n &a&b\\\\\n &c&d\\\\\n &e&f\\\\\n\\end{tabular}\n";

	QTest::newRow("add second col")
		<< "\\begin{tabular}{xy}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 1
		<< "\\begin{tabular}{xly}\na& &b\\\\\nc& &d\\\\\ne& &f\\\\\n\\end{tabular}\n";

    QTest::newRow("add second col, tabularx")
        << "\\begin{tabularx}{\\linewidth}{xy}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabularx}\n"
        << 1 << 1
        << "\\begin{tabularx}{\\linewidth}{xly}\na& &b\\\\\nc& &d\\\\\ne& &f\\\\\n\\end{tabularx}\n";

    QTest::newRow("add second col tnl")
        << "\\begin{tabular}{xy}\na&b\\tabularnewline\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
        << 1 << 1
        << "\\begin{tabular}{xly}\na& &b\\tabularnewline\nc& &d\\\\\ne& &f\\\\\n\\end{tabular}\n";

	QTest::newRow("add third col")
		<< "\\begin{tabular}{xy}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 2
		<< "\\begin{tabular}{xyl}\na&b& \\\\\nc&d& \\\\\ne&f& \\\\\n\\end{tabular}\n";
    // tests for tabu/longtabu
    QTest::newRow("add first col tabu")
        << "\\begin{tabu}{xy}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabu}\n"
        << 1 << 0
        << "\\begin{tabu}{lxy}\n &a&b\\\\\n &c&d\\\\\n &e&f\\\\\n\\end{tabu}\n";
    QTest::newRow("add second col tabu")
        << "\\begin{tabu}{xy}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabu}\n"
        << 1 << 1
        << "\\begin{tabu}{xly}\na& &b\\\\\nc& &d\\\\\ne& &f\\\\\n\\end{tabu}\n";
    QTest::newRow("add second col tabu with to")
        << "\\begin{tabu}to 2cm {xy}\na&b\\tabularnewline\nc&d\\\\\ne&f\\\\\n\\end{tabu}\n"
        << 1 << 1
        << "\\begin{tabu}to 2cm {xly}\na& &b\\tabularnewline\nc& &d\\\\\ne& &f\\\\\n\\end{tabu}\n";
    QTest::newRow("add second col tabu with spread")
        << "\\begin{tabu} spread 2cm {xy}\na&b\\tabularnewline\nc&d\\\\\ne&f\\\\\n\\end{tabu}\n"
        << 1 << 1
        << "\\begin{tabu} spread 2cm {xly}\na& &b\\tabularnewline\nc& &d\\\\\ne& &f\\\\\n\\end{tabu}\n";
    // tests for tblr
    QTest::newRow("add first col tblr")
        << "\\begin{tblr}{xy}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tblr}\n"
        << 1 << 0
        << "\\begin{tblr}{lxy}\n &a&b\\\\\n &c&d\\\\\n &e&f\\\\\n\\end{tblr}\n";
    QTest::newRow("add second col tblr")
        << "\\begin{tblr}{xy}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tblr}\n"
        << 1 << 1
        << "\\begin{tblr}{xly}\na& &b\\\\\nc& &d\\\\\ne& &f\\\\\n\\end{tblr}\n";
    QTest::newRow("add first col tblr with new interface")
        << "\\begin{tblr}{colspec={xy}}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tblr}\n"
        << 1 << 0
        << "\\begin{tblr}{colspec={lxy}}\n &a&b\\\\\n &c&d\\\\\n &e&f\\\\\n\\end{tblr}\n";
    QTest::newRow("add first col tblr with new interface, comma separated")
        << "\\begin{tblr}{colspec={xy},hlines}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tblr}\n"
        << 1 << 0
        << "\\begin{tblr}{colspec={lxy},hlines}\n &a&b\\\\\n &c&d\\\\\n &e&f\\\\\n\\end{tblr}\n";
    QTest::newRow("add first col tblr with new interface,mulitline")
        << "\\begin{tblr}{\ncolspec={xy}\n}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tblr}\n"
        << 3 << 0
        << "\\begin{tblr}{\ncolspec={lxy}\n}\n &a&b\\\\\n &c&d\\\\\n &e&f\\\\\n\\end{tblr}\n";
    QTest::newRow("add first col tblr, mulitline cell")
        << "\\begin{tblr}{xy}\na&{b\\\\b}\\\\\nc&d\\\\\ne&f\\\\\n\\end{tblr}\n"
        << 3 << 0
        << "\\begin{tblr}{lxy}\n &a&{b\\\\b}\\\\\n &c&d\\\\\n &e&f\\\\\n\\end{tblr}\n";
    QTest::newRow("add second col tblr, mulitline cell")
        << "\\begin{tblr}{xy}\na&{b\\\\b}\\\\\nc&d\\\\\ne&f\\\\\n\\end{tblr}\n"
        << 3 << 2
        << "\\begin{tblr}{xyl}\na&{b\\\\b}& \\\\\nc&d& \\\\\ne&f& \\\\\n\\end{tblr}\n";
    // incorrect table
    QTest::newRow("add second col, incorrect tabular")
        << "\\begin{tabular}{xy}\na&b\\\\\nc&d&e\\\\\nf\\\\\n\\end{tabular}\n"
        << 1 << 1
        << "\\begin{tabular}{xly}\na& &b\\\\\nc& &d&e\\\\\nf& \\\\\n\\end{tabular}\n";
    QTest::newRow("add third col, incorrect tabular")
        << "\\begin{tabular}{xy}\na&b\\\\\nc&d&e\\\\\nf\\\\\n\\end{tabular}\n"
        << 1 << 2
        << "\\begin{tabular}{xyl}\na&b& \\\\\nc&d& &e\\\\\nf& \\\\\n\\end{tabular}\n";
    // lazy tables
    QTest::newRow("add second col, last \\\\ missing")
        << "\\begin{tabular}{xy}\na&b\\\\\nc&d\\\\\ne&f\n\\end{tabular}\n"
        << 1 << 1
        << "\\begin{tabular}{xly}\na& &b\\\\\nc& &d\\\\\ne& &f\n\\end{tabular}\n";
    QTest::newRow("add third col, last \\\\ missing")
        << "\\begin{tabular}{xy}\na&b\\\\\nc&d\\\\\ne&f\n\\end{tabular}\n"
        << 1 << 2
        << "\\begin{tabular}{xyl}\na&b& \\\\\nc&d& \\\\\ne&f& \n\\end{tabular}\n";
    QTest::newRow("add 2nd col, single col, last \\\\ missing")
        << "\\begin{tabular}{x}\na\\\\\nc\\\\\ne\n\\end{tabular}\n"
        << 1 << 1
        << "\\begin{tabular}{xl}\na& \\\\\nc& \\\\\ne& \n\\end{tabular}\n";
    // hline
    QTest::newRow("add first col, hline")
        << "\\begin{tabular}{xy}\na&b\\\\ \\hline\nc&d\\\\\ne&f\\\\ \\hline\n\\end{tabular}\n"
        << 1 << 0
        << "\\begin{tabular}{lxy}\n &a&b\\\\ \\hline\n &c&d\\\\\n &e&f\\\\ \\hline\n\\end{tabular}\n";
    QTest::newRow("add second col, hline")
        << "\\begin{tabular}{xy}\na&b\\\\ \\hline\nc&d\\\\\ne&f\\\\ \\hline\n\\end{tabular}\n"
        << 1 << 1
        << "\\begin{tabular}{xly}\na& &b\\\\ \\hline\nc& &d\\\\\ne& &f\\\\ \\hline\n\\end{tabular}\n";
    // multi lines per row
    QTest::newRow("add col 0, row over multiple lines")
        << "\\begin{tabular}{ll}\na&\nb\\\\\nc\n&\nd\\\\\ne&f\\\\\n\\end{tabular}\n"
        << 1 << 0
        << "\\begin{tabular}{lll}\n &a&\nb\\\\\n &c\n&\nd\\\\\n &e&f\\\\\n\\end{tabular}\n";

    QTest::newRow("add col 1, row over multiple lines")
        << "\\begin{tabular}{ll}\na&\nb\\\\\nc\n&\nd\\\\\ne&f\\\\\n\\end{tabular}\n"
        << 1 << 1
        << "\\begin{tabular}{lll}\na& &\nb\\\\\nc\n& &\nd\\\\\ne& &f\\\\\n\\end{tabular}\n";
}
void TableManipulationTest::addCol(){
	QFETCH(QString, text);
	QFETCH(int, row);
	QFETCH(int, col);
	QFETCH(QString, newText);

    //add latex preample
    text="\\usepackage{tabularx,tabu,tabularray}\n\\begin{document}\n"+text+"\n\\end{document}\n";
    newText="\\usepackage{tabularx,tabu,tabularray}\n\\begin{document}\n"+newText+"\n\\end{document}\n";
    row+=2; // adjust row number to account for preamble
	ed->setText(text, false);
    LatexDocument *doc=dynamic_cast<LatexDocument*>(ed->document());
    doc->synChecker.waitForQueueProcess();
    StackEnvironment stackEnv;
    doc->getEnv(row,stackEnv);
    int i=LatexTables::inTableEnv(stackEnv);
    QVERIFY(i>=0);
    if (i<0) return;
    LatexTables::addColumn(stackEnv[i], row, col);

    ed->document()->setLineEndingDirect(QDocument::Unix,true);
	QEQUAL(ed->document()->text(), newText);
	
}

void TableManipulationTest::addRow_data(){
	QTest::addColumn<QString>("text");
	QTest::addColumn<int>("row");
	QTest::addColumn<int>("col");
	QTest::addColumn<QString>("newText");

	//-------------cursor without selection--------------
	QTest::newRow("add row")
		<< "\\begin{tabular}{ll}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 0
		<< "\\begin{tabular}{ll}\na&b\\\\\n & \\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n";

    QTest::newRow("add row tnl")
        << "\\begin{tabular}{ll}\na&b\\tabularnewline\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
        << 1 << 0
        << "\\begin{tabular}{ll}\na&b\\tabularnewline\n & \\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n";

	QTest::newRow("add row, cursor at end of line")
		<< "\\begin{tabular}{ll}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 5
		<< "\\begin{tabular}{ll}\na&b\\\\\n & \\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n";

    QTest::newRow("add row 2")
		<< "\\begin{tabular}{ll}\na&b\\\\c&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 0
		<< "\\begin{tabular}{ll}\na&b\\\\\n & \\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n";

    QTest::newRow("add row 3")
		<< "\\begin{tabular}{ll}\na&b\\\\c&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 6
		<< "\\begin{tabular}{ll}\na&b\\\\c&d\\\\\n & \\\\\ne&f\\\\\n\\end{tabular}\n";

    QTest::newRow("add row, no final \\\\")
        << "\\begin{tabular}{ll}\na&b\\\\\nc&d\\\\\ne&f\n\\end{tabular}\n"
        << 3 << 0
        << "\\begin{tabular}{ll}\na&b\\\\\nc&d\\\\\ne&f\\\\\n & \\\\\n\\end{tabular}\n";

}
void TableManipulationTest::addRow(){
	QFETCH(QString, text);
	QFETCH(int, row);
	QFETCH(int, col);
	QFETCH(QString, newText);

	ed->setText(text, false);
	ed->setCursorPosition(row,col);
	QDocumentCursor c(ed->cursor());
	LatexTables::addRow(c,2);

    ed->document()->setLineEndingDirect(QDocument::Unix,true);
	QEQUAL(ed->document()->text(), newText);

}

void TableManipulationTest::remCol_data(){
	QTest::addColumn<QString>("text");
	QTest::addColumn<int>("row");
	QTest::addColumn<int>("col");
	QTest::addColumn<QString>("newText");

	//-------------cursor without selection--------------
    QTest::newRow("rem col 0")
		<< "\\begin{tabular}{xy}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 0
		<< "\\begin{tabular}{y}\nb\\\\\nd\\\\\nf\\\\\n\\end{tabular}\n";

	QTest::newRow("rem col 1")
		<< "\\begin{tabular}{xy}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 1
		<< "\\begin{tabular}{x}\na\\\\\nc\\\\\ne\\\\\n\\end{tabular}\n";

    QTest::newRow("rem col 2")
        << "\\begin{tabular}{xyz}\na&b&c\\\\\nc&d&e\\\\\ne&f&g\\\\\n\\end{tabular}\n"
        << 1 << 2
        << "\\begin{tabular}{xy}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n";

    QTest::newRow("rem col 1, tabularx")
        << "\\begin{tabularx}{\\linewidth}{xy}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
        << 1 << 1
        << "\\begin{tabularx}{\\linewidth}{x}\na\\\\\nc\\\\\ne\\\\\n\\end{tabular}\n";

    QTest::newRow("rem col 1, tabularnewline")
        << "\\begin{tabular}{xy}\na&b\\tabularnewline\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
        << 1 << 1
        << "\\begin{tabular}{x}\na\\tabularnewline\nc\\\\\ne\\\\\n\\end{tabular}\n";

	QTest::newRow("rem col 0, multicolumn")
		<< "\\begin{tabular}{ll}\na&b\\\\\n\\multicolumn{2}{c}{txt}\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 0
		<< "\\begin{tabular}{l}\nb\\\\\n\\multicolumn{1}{c}{txt}\\\\\nf\\\\\n\\end{tabular}\n";

	QTest::newRow("rem col 1, multicolumn")
		<< "\\begin{tabular}{ll}\na&b\\\\\n\\multicolumn{2}{c}{txt}\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 1
		<< "\\begin{tabular}{l}\na\\\\\n\\multicolumn{1}{c}{txt}\\\\\ne\\\\\n\\end{tabular}\n";

	QTest::newRow("rem col 1, multicolumn plus col")
		<< "\\begin{tabular}{ll}\na&b\\\\\n\\multicolumn{2}{c}{txt}&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 1
		<< "\\begin{tabular}{l}\na\\\\\n\\multicolumn{1}{c}{txt}&d\\\\\ne\\\\\n\\end{tabular}\n";

	QTest::newRow("rem col 2, multicolumn")
        << "\\begin{tabular}{lll}\na&b&c\\\\\n\\multicolumn{2}{c}{txt}&d\\\\\ne&f&g\\\\\n\\end{tabular}\n"
		<< 1 << 2
		<< "\\begin{tabular}{ll}\na&b\\\\\n\\multicolumn{2}{c}{txt}\\\\\ne&f\\\\\n\\end{tabular}\n";

	QTest::newRow("rem col 0, row over multiple lines")
		<< "\\begin{tabular}{ll}\na&\nb\\\\\nc\n&\nd\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 0
		<< "\\begin{tabular}{l}\n\nb\\\\\n\nd\\\\\nf\\\\\n\\end{tabular}\n";

	QTest::newRow("rem col 1, row over multiple lines")
		<< "\\begin{tabular}{ll}\na&\nb\\\\\nc\n&\nd\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 1
		<< "\\begin{tabular}{l}\na\\\\\nc\n\\\\\ne\\\\\n\\end{tabular}\n";

    QTest::newRow("rem col 0 containing \\hline")
            << "\\begin{tabular}{ll}\na&b\\\\ \\hline\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
            << 1 << 0
            << "\\begin{tabular}{l}\nb\\\\ \\hline\nd\\\\\nf\\\\\n\\end{tabular}\n";

    QTest::newRow("rem col 0 containing \\hline 2")
			<< "\\begin{tabular}{ll}\na&b\\\\ \\hline\nc&d\\\\\ne&f\\\\ \\hline\n\\end{tabular}\n"
			<< 1 << 0
			<< "\\begin{tabular}{l}\nb\\\\ \\hline\nd\\\\\nf\\\\ \\hline\n\\end{tabular}\n";

    QTest::newRow("rem col 0 containing \\hline 3")
			<< "\\begin{tabular}{ll}\na&b\\\\ \\hline\nc&d\\\\\ne&f\\\\\\hline\n\\end{tabular}\n"
			<< 1 << 0
            << "\\begin{tabular}{l}\nb\\\\ \\hline\nd\\\\\nf\\\\\\hline\n\\end{tabular}\n";

	QTest::newRow("rem last col")
			<< "\\begin{tabular}{l}\na\\\\ \\hline\nd\\\\\nf\\\\\\hline\n\\end{tabular}\n"
			<< 1 << 0
            << "\\begin{tabular}{}\n\\\\ \\hline\n\\\\\n\\\\\\hline\n\\end{tabular}\n";

    QTest::newRow("rem last col, no final \\\\")
        << "\\begin{tabular}{ll}\na&b\\\\ \\hline\nc&d\\\\\ne&f\n\\end{tabular}\n"
        << 1 << 1
        << "\\begin{tabular}{l}\na\\\\ \\hline\nc\\\\\ne\n\\end{tabular}\n";
    // tests for tabu/longtabu
    QTest::newRow("rem col 0 tabu")
        << "\\begin{tabu}{xy}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabu}\n"
        << 1 << 0
        << "\\begin{tabu}{y}\nb\\\\\nd\\\\\nf\\\\\n\\end{tabu}\n";
    QTest::newRow("rem col 1 tabu")
        << "\\begin{tabu}{xy}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabu}\n"
        << 1 << 1
        << "\\begin{tabu}{x}\na\\\\\nc\\\\\ne\\\\\n\\end{tabu}\n";
    QTest::newRow("rem col 1 tabu with to")
        << "\\begin{tabu}to 2cm {xy}\na&b\\tabularnewline\nc&d\\\\\ne&f\\\\\n\\end{tabu}\n"
        << 1 << 1
        << "\\begin{tabu}to 2cm {x}\na\\tabularnewline\nc\\\\\ne\\\\\n\\end{tabu}\n";
    QTest::newRow("rem col 1 tabu with spread")
        << "\\begin{tabu} spread 2cm {xy}\na&b\\tabularnewline\nc&d\\\\\ne&f\\\\\n\\end{tabu}\n"
        << 1 << 1
        << "\\begin{tabu} spread 2cm {x}\na\\tabularnewline\nc\\\\\ne\\\\\n\\end{tabu}\n";
    // tests for tblr
    QTest::newRow("rem col 0 tblr")
        << "\\begin{tblr}{xy}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tblr}\n"
        << 1 << 0
        << "\\begin{tblr}{y}\nb\\\\\nd\\\\\nf\\\\\n\\end{tblr}\n";
    QTest::newRow("rem col 1 tblr")
        << "\\begin{tblr}{xy}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tblr}\n"
        << 1 << 1
        << "\\begin{tblr}{x}\na\\\\\nc\\\\\ne\\\\\n\\end{tblr}\n";
    QTest::newRow("rem col 0 tblr with new interface")
        << "\\begin{tblr}{colspec={xy}}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tblr}\n"
        << 1 << 0
        << "\\begin{tblr}{colspec={y}}\nb\\\\\nd\\\\\nf\\\\\n\\end{tblr}\n";
    QTest::newRow("rem col 1 tblr with new interface")
        << "\\begin{tblr}{colspec={xy}}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tblr}\n"
        << 1 << 1
        << "\\begin{tblr}{colspec={x}}\na\\\\\nc\\\\\ne\\\\\n\\end{tblr}\n";
    QTest::newRow("rem col 0 tblr with new interface, comma separated")
        << "\\begin{tblr}{colspec={xy},hlines}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tblr}\n"
        << 1 << 0
        << "\\begin{tblr}{colspec={y},hlines}\nb\\\\\nd\\\\\nf\\\\\n\\end{tblr}\n";
    QTest::newRow("rem col 0 tblr with new interface,mulitline")
        << "\\begin{tblr}{\ncolspec={xy}\n}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tblr}\n"
        << 3 << 0
        << "\\begin{tblr}{\ncolspec={y}\n}\nb\\\\\nd\\\\\nf\\\\\n\\end{tblr}\n";
    QTest::newRow("rem col 1 tblr, multiline cell")
        << "\\begin{tblr}{xy}\na&{b\\\\b}\\\\\n{c\\\\c}&d\\\\\ne&f\\\\\n\\end{tblr}\n"
        << 1 << 1
        << "\\begin{tblr}{x}\na\\\\\n{c\\\\c}\\\\\ne\\\\\n\\end{tblr}\n";
    // faulty tables
    QTest::newRow("rem col 1, less/more columns")
        << "\\begin{tabular}{xy}\na&b\\\\\nc\\\\\ne&f&g\\\\\n\\end{tabular}\n"
        << 1 << 1
        << "\\begin{tabular}{x}\na\\\\\nc\\\\\ne&g\\\\\n\\end{tabular}\n";

}
void TableManipulationTest::remCol(){
	QFETCH(QString, text);
	QFETCH(int, row);
	QFETCH(int, col);
	QFETCH(QString, newText);

    //add latex preample
    text="\\usepackage{tabularx,tabu,tabularray}\n\\begin{document}\n"+text+"\n\\end{document}\n";
    newText="\\usepackage{tabularx,tabu,tabularray}\n\\begin{document}\n"+newText+"\n\\end{document}\n";
    row+=2; // adjust row number to account for preamble

	ed->setText(text, false);
    LatexDocument *doc=dynamic_cast<LatexDocument*>(ed->document());
    doc->synChecker.waitForQueueProcess();
    StackEnvironment stackEnv;
    doc->getEnv(row,stackEnv);
    int i=LatexTables::inTableEnv(stackEnv);
    QVERIFY(i>=0);
    if (i<0) return;
    LatexTables::removeColumn(stackEnv[i], row, col);
	
	QEQUAL(ed->document()->text(),newText);
	
}

void TableManipulationTest::remRow_data(){
	QTest::addColumn<QString>("text");
	QTest::addColumn<int>("row");
	QTest::addColumn<int>("col");
	QTest::addColumn<QString>("newText");

	//-------------cursor without selection--------------
	QTest::newRow("rem row")
		<< "\\begin{tabular}{ll}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 0
		<< "\\begin{tabular}{ll}\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n";

	QTest::newRow("rem row, cursor at end of line")
		<< "\\begin{tabular}{ll}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 5
		<< "\\begin{tabular}{ll}\na&b\\\\\ne&f\\\\\n\\end{tabular}\n";

	QTest::newRow("rem row, second row")
		<< "\\begin{tabular}{ll}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 2 << 0
		<< "\\begin{tabular}{ll}\na&b\\\\\ne&f\\\\\n\\end{tabular}\n";

    QTest::newRow("rem row, second row, tnl")
        << "\\begin{tabular}{ll}\na&b\\tabularnewline\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
        << 2 << 0
        << "\\begin{tabular}{ll}\na&b\\tabularnewline\ne&f\\\\\n\\end{tabular}\n";


	QTest::newRow("rem row, third row")
		<< "\\begin{tabular}{ll}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 3 << 0
		<< "\\begin{tabular}{ll}\na&b\\\\\nc&d\\\\\n\\end{tabular}\n";

	QTest::newRow("rem row, multi rows in one line 1")
		<< "\\begin{tabular}{ll}\na&b\\\\c&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 0
		<< "\\begin{tabular}{ll}\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n";

	QTest::newRow("rem row, multi rows in one line 2")
		<< "\\begin{tabular}{ll}\na&b\\\\c&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 6
		<< "\\begin{tabular}{ll}\na&b\\\\\ne&f\\\\\n\\end{tabular}\n";

	QTest::newRow("rem row, multi rows in one line 3")
		<< "\\begin{tabular}{ll}\na&b\\\\c&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 2 << 0
		<< "\\begin{tabular}{ll}\na&b\\\\c&d\\\\\n\\end{tabular}\n";

	QTest::newRow("rem row, multi rows in one line 4")
		<< "\\begin{tabular}{ll}\na&b\\\\c&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 2 << 4
		<< "\\begin{tabular}{ll}\na&b\\\\c&d\\\\\n\\end{tabular}\n";

    QTest::newRow("rem row, multi rows in one line 3 b")
		<< "\\begin{tabular}{ll}\na&b\\\\c&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 2 << 5
		<< "\\begin{tabular}{ll}\na&b\\\\c&d\\\\\ne&f\\\\\n\\end{tabular}\n";

    QTest::newRow("rem row, multi rows in one line 4 b")
		<< "\\begin{tabular}{ll}\na&b\\\\c&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 3 << 6
		<< "\\begin{tabular}{ll}\na&b\\\\c&d\\\\\ne&f\\\\\n\\end{tabular}\n";


}
void TableManipulationTest::remRow(){
	QFETCH(QString, text);
	QFETCH(int, row);
	QFETCH(int, col);
	QFETCH(QString, newText);

	ed->setText(text, false);
	ed->setCursorPosition(row,col);
	QDocumentCursor c(ed->cursor());
	LatexTables::removeRow(c);

	QEQUAL(ed->document()->text(), newText);

}

void TableManipulationTest::getCol_data(){
	QTest::addColumn<QString>("text");
	QTest::addColumn<int>("row");
	QTest::addColumn<int>("col");
	QTest::addColumn<int>("colFound");

	//-------------cursor without selection--------------
    QTest::newRow("col 0")
		<< "\\begin{tabular}{ll}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 0
		<< 0;

    QTest::newRow("col 0a")
		<< "\\begin{tabular}{ll}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 1
		<< 0;

	QTest::newRow("col 1")
		<< "\\begin{tabular}{ll}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 2
		<< 1;

    QTest::newRow("col 1a")
		<< "\\begin{tabular}{ll}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 3
		<< 1;

    QTest::newRow("col 1b")
		<< "\\begin{tabular}{ll}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 4
        << -1;

	QTest::newRow("col -1")
		<< "\\begin{tabular}{ll}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 5
        << 0;

	QTest::newRow("row 2,col 1")
		<< "\\begin{tabular}{ll}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 2 << 0
		<< 0;

	QTest::newRow("row 3,col 1")
		<< "\\begin{tabular}{ll}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 3 << 0
		<< 0;

	QTest::newRow("row 2,col 2")
		<< "\\begin{tabular}{ll}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 3 << 2
		<< 1;

	QTest::newRow("row 2,col -1")
		<< "\\begin{tabular}{ll}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
        << 3 << 4
		<< -1;

	QTest::newRow("row 1,col 1,multi row per line")
		<< "\\begin{tabular}{ll}\na&b\\\\c&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 0
		<< 0;

    QTest::newRow("row 1,col 1,multi row per line b")
		<< "\\begin{tabular}{ll}\na&b\\\\c&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 5
		<< 0;

	QTest::newRow("row 1,col 2,multi row per line")
		<< "\\begin{tabular}{ll}\na&b\\\\c&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 7
		<< 1;

	QTest::newRow("no row")
		<< "\\begin{tabular}{ll}\na\n\\end{tabular}\n"
		<< 1 << 1
		<< 0;

	QTest::newRow("no row, col 1")
		<< "\\begin{tabular}{ll}\na&b\n\\end{tabular}\n"
		<< 1 << 2
		<< 1;

    QTest::newRow("no row, col 1 b")
		<< "\\begin{tabular}{ll}\na&b\\\\\nc&d\n\\end{tabular}\n"
		<< 2 << 2
		<< 1;
    // special tblr
    QTest::newRow("tblr")
        << "\\begin{tblr}{ll}\na&b\\\\\nc&d\n\\end{tblr}\n"
        << 2 << 2
        << 1;
    QTest::newRow("tblr, multi line cell 1")
        << "\\begin{tblr}{ll}\na&{b\\\\b}\\\\\nc&d\n\\end{tblr}\n"
        << 1 << 2
        << 1;
    QTest::newRow("tblr, multi line cell 2")
        << "\\begin{tblr}{ll}\na&{b\\\\b}\\\\\nc&d\n\\end{tblr}\n"
        << 1 << 8
        << 1;
    QTest::newRow("tblr, multi line cell 3")
        << "\\begin{tblr}{ll}\na&{b\\\\b}\\\\\nc&d\n\\end{tblr}\n"
        << 1 << 5
        << 1;
    QTest::newRow("tblr, multi line cell 4")
        << "\\begin{tblr}{ll}\na&{b\\\\b}\\\\\nc&d\n\\end{tblr}\n"
        << 1 << 10
        << 0;
    QTest::newRow("tblr, multi line cell 5")
        << "\\begin{tblr}{ll}\na&\n{b\\\\b}\\\\\nc&d\n\\end{tblr}\n"
        << 2 << 0
        << 1;
}
void TableManipulationTest::getCol(){
	QFETCH(QString, text);
	QFETCH(int, row);
	QFETCH(int, col);
	QFETCH(int, colFound);

    //add latex preample
    text="\\usepackage{tabularray}\n\\begin{document}\n"+text+"\n\\end{document}\n";
    row+=2; // adjust row number to account for preamble

	ed->setText(text, false);
	ed->setCursorPosition(row,col);
	QDocumentCursor c(ed->cursor());
    LatexDocument *doc=dynamic_cast<LatexDocument*>(ed->document());
    doc->synChecker.waitForQueueProcess();
    StackEnvironment stackEnv;
    doc->getEnv(row,stackEnv);
    int i=LatexTables::inTableEnv(stackEnv);
    QVERIFY(i>=0);
    if (i<0) return;
    int nc=LatexTables::getColumn(c,stackEnv[i]);

	QEQUAL(nc,colFound);

}

void TableManipulationTest::getNumberOfCol_data(){
	QTest::addColumn<QString>("text");
	QTest::addColumn<int>("row");
	QTest::addColumn<int>("col");
	QTest::addColumn<int>("colFound");

	//-------------cursor without selection--------------
	QTest::newRow("cols 2")
		<< "\\begin{tabular}{ll}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 0
		<< 2;

	QTest::newRow("cols 4")
		<< "\\begin{tabular}{|l|l|cc}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 2 << 0
		<< 4;

    QTest::newRow("tabularx")
        << "\\usepackage{tabularx}\n\\begin{tabularx}{\\linewidth}{|l|l|cc}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabularx}\n"
        << 2 << 0
        << 4;

	QTest::newRow("cols 0")
		<< "\\begin{tabular}{}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 0
		<< 0;

	QTest::newRow("spaced in definition")
		<< "\\begin{tabular}{l l c}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 0
		<< 3;

	QTest::newRow("p")
		<< "\\begin{tabular}{llp{3cm}}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 0
		<< 3;

	QTest::newRow("m")
		<< "\\begin{tabular}{llm{3cm}}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 0
		<< 3;

	QTest::newRow("col commands (array)")
		<< "\\begin{tabular}{>{\\bfseries}ll}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 0
		<< 2;

	QTest::newRow("no Table")
		<< "test\na&b\\\\\nc&d\\\\\ne&f\\\\\ntest\n"
		<< 1 << 0
		<< -1;

	QTest::newRow("separators")
		<< "\\begin{tabular}{|l|l|@{ll}cc}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 2 << 0
		<< 4;

	QTest::newRow("multipliers")
		<< "\\begin{tabular}{|l|l|@{ll}c*{2}{lc}}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 2 << 0
		<< 7;

	QTest::newRow("multipliers, nested")
		<< "\\begin{tabular}{|l|l|@{ll}c*{2}{*{2}{l}}}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 2 << 0
        << 7;
    QTest::newRow("colspec")
        << "\\begin{tblr}{colspec={|l|l|@{ll}c*{2}{*{2}{l}}}}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tblr}\n"
        << 2 << 0
        << 7;
    QTest::newRow("colspec2")
        << "\\begin{tblr}{\n\tcolspec={|l|l|@{ll}c*{2}{*{2}{l}}}}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tblr}\n"
        << 3 << 0
        << 7;
    QTest::newRow("colspec3")
        << "\\begin{tblr}{\n\tcolspec={|l|l|@{ll}c*{2}{*{2}{l}}},width={3cm}}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tblr}\n"
        << 3 << 0
        << 7;
    QTest::newRow("colspec4")
        << "\\begin{tblr}{\n\tcolspec= {Q[1,r,m]Q[1,l,m]},width = 0.4\\linewidth,column{1} = {font=\bfseries}}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tblr}\n"
        << 3 << 0
        << 2;

}
void TableManipulationTest::getNumberOfCol(){
	QFETCH(QString, text);
	QFETCH(int, row);
	QFETCH(int, col);
	QFETCH(int, colFound);

	ed->setText(text, false);
	ed->setCursorPosition(row,col);
	QDocumentCursor c(ed->cursor());
	int nc=LatexTables::getNumberOfColumns(c);

	QEQUAL(nc,colFound);

}

void TableManipulationTest::findNextToken_data(){
	QTest::addColumn<QString>("text");
	QTest::addColumn<int>("row");
	QTest::addColumn<int>("col");
	QTest::addColumn<int>("result");
	QTest::addColumn<int>("newRow");
	QTest::addColumn<int>("newCol");

	QTest::newRow("find &")
		<< "\\begin{tabular}{ll}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 0
		<< 2 << 1 << 2;

	QTest::newRow("find \\")
		<< "\\begin{tabular}{ll}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 2
		<< 0 << 1 << 5;

	QTest::newRow("find \\, multi line")
		<< "\\begin{tabular}{ll}\na&\nb\n\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 2
		<< 0 << 3 << 2;

	QTest::newRow("run into \\end")
		<< "\\begin{tabular}{ll}\na\ncd\n\\end{tabular}\n"
		<< 1 << 2
		<< -2 << 3 << 0;

	QTest::newRow("run into eof")
		<< "\\begin{tabular}{ll}\na\ncd\nnd{tabular}\n"
		<< 1 << 2
		<< -1 << 4 << 0;

}
void TableManipulationTest::findNextToken(){
	QFETCH(QString, text);
	QFETCH(int, row);
	QFETCH(int, col);
	QFETCH(int, result);
	QFETCH(int, newRow);
	QFETCH(int, newCol);


	ed->setText(text, false);
	ed->setCursorPosition(row,col);
	QDocumentCursor c(ed->cursor());
	QStringList tokens;
	tokens<<"\\\\"<<"\\&"<<"&";
	int res=LatexTables::findNextToken(c,tokens);

	QEQUAL(res,result);
	QEQUAL(c.lineNumber(),newRow);
	QEQUAL(c.columnNumber(),newCol);

}

void TableManipulationTest::findNextTokenBackwards_data(){
	QTest::addColumn<QString>("text");
	QTest::addColumn<int>("row");
	QTest::addColumn<int>("col");
	QTest::addColumn<int>("result");
	QTest::addColumn<int>("newRow");
	QTest::addColumn<int>("newCol");

	QTest::newRow("find &")
		<< "\\begin{tabular}{ll}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 3
		<< 2 << 1 << 1;

	QTest::newRow("find \\")
		<< "\\begin{tabular}{ll}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 2 << 1
		<< 0 << 1 << 3;

	QTest::newRow("find &, multi line")
		<< "\\begin{tabular}{ll}\na&\nb\n\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 3 << 0
		<< 2 << 1 << 1;

	QTest::newRow("run into \\begin")
		<< "\\begin{tabular}{ll}\na\ncd\n\\end{tabular}\n"
		<< 1 << 1
		<< -2 << 0 << 19;

	QTest::newRow("run into eof")
		<< "egin{tabular}{ll}\na\ncd\nnd{tabular}\n"
		<< 1 << 1
		<< -1 << 0 << 17;

}
void TableManipulationTest::findNextTokenBackwards(){
	QFETCH(QString, text);
	QFETCH(int, row);
	QFETCH(int, col);
	QFETCH(int, result);
	QFETCH(int, newRow);
	QFETCH(int, newCol);


	ed->setText(text, false);
	ed->setCursorPosition(row,col);
	QDocumentCursor c(ed->cursor());
	QStringList tokens;
	tokens<<"\\\\"<<"\\&"<<"&";
	int res=LatexTables::findNextToken(c,tokens,false,true);

	QEQUAL(res,result);
	QEQUAL(c.lineNumber(),newRow);
	QEQUAL(c.columnNumber(),newCol);

}

void TableManipulationTest::addHLine_data(){
	QTest::addColumn<QString>("text");
	QTest::addColumn<int>("row");
	QTest::addColumn<int>("col");
	QTest::addColumn<int>("numberOfLines");
	QTest::addColumn<bool>("remove");
	QTest::addColumn<QString>("newText");

	//-------------cursor without selection--------------
	QTest::newRow("add to all")
		<< "\\begin{tabular}{ll}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 0 << -1 << false
		<< "\\begin{tabular}{ll}\na&b\\\\ \\hline\nc&d\\\\ \\hline\ne&f\\\\ \\hline\n\\end{tabular}\n";

    QTest::newRow("add to all tnl")
        << "\\begin{tabular}{ll}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
        << 1 << 0 << -1 << false
        << "\\begin{tabular}{ll}\na&b\\\\ \\hline\nc&d\\\\ \\hline\ne&f\\\\ \\hline\n\\end{tabular}\n";

	QTest::newRow("add to 2 (in single line)")
		<< "\\begin{tabular}{ll}\na&b\\\\c&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 0 << 2 << false
		<< "\\begin{tabular}{ll}\na&b\\\\ \\hline\nc&d\\\\ \\hline\ne&f\\\\\n\\end{tabular}\n";

	QTest::newRow("remove all, none present")
		<< "\\begin{tabular}{ll}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 0 << -1 << true
		<< "\\begin{tabular}{ll}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n";

	QTest::newRow("remove all")
		<< "\\begin{tabular}{ll}\na&b\\\\ \\hline\nc&d\\\\ \\hline\ne&f\\\\ \\hline\n\\end{tabular}\n"
		<< 1 << 0 << -1 << true
		<< "\\begin{tabular}{ll}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n";

	QTest::newRow("remove all, missing hlines")
		<< "\\begin{tabular}{ll}\na&b\\\\\nc&d\\\\ \\hline\ne&f\\\\ \\hline\n\\end{tabular}\n"
		<< 1 << 0 << -1 << true
		<< "\\begin{tabular}{ll}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n";

	QTest::newRow("remove some")
		<< "\\begin{tabular}{ll}\na&b\\\\ \\hline\nc&d\\\\\\hline\ne&f\\\\ \\hline\n\\end{tabular}\n"
		<< 1 << 0 << 2 << true
		<< "\\begin{tabular}{ll}\na&b\\\\\nc&d\\\\\ne&f\\\\ \\hline\n\\end{tabular}\n";

}
void TableManipulationTest::addHLine(){
	QFETCH(QString, text);
	QFETCH(int, row);
	QFETCH(int, col);
	QFETCH(int, numberOfLines);
	QFETCH(bool, remove);
	QFETCH(QString, newText);

	ed->setText(text, false);
	ed->setCursorPosition(row,col);
	QDocumentCursor c(ed->cursor());
	LatexTables::addHLine(c,numberOfLines,remove);

    ed->document()->setLineEndingDirect(QDocument::Unix,true);
    QString result=ed->document()->text();
    QEQUAL(result, newText);

}

void TableManipulationTest::splitCol_data(){
	QTest::addColumn<QString>("text");
	QTest::addColumn<int>("colFound");

	//-------------cursor without selection--------------
	QTest::newRow("cols 2")
		<< "ll"
		<< 2;

	QTest::newRow("cols 4")
		<< "|l|l|cc"
		<< 4;

	QTest::newRow("cols 0")
		<< ""
		<< 0;

	QTest::newRow("spaced in definition")
		<< "l l c"
		<< 3;

	QTest::newRow("p")
		<< "llp{3cm}"
		<< 3;

    QTest::newRow("p 2")
		<< "llm{3cm}"
		<< 3;

	QTest::newRow("col commands (array)")
		<< ">{\\bfseries}ll"
		<< 2;

	QTest::newRow("separators")
		<< "|l|l|@{ll}cc"
		<< 4;

	QTest::newRow("multipliers")
		<< "|l|l|@{ll}c*{2}{lc}"
        << 7;
    QTest::newRow("colspec")
        << "|X[2,l]|X[3,l]|"
        << 2;
    QTest::newRow("colspec2")
        << "|Q[2,l]|Q[3,l]|"
        << 2;


}
void TableManipulationTest::splitCol(){
	QFETCH(QString, text);
	QFETCH(int, colFound);


	QStringList res=LatexTables::splitColDef(text);

	QEQUAL(res.count(),colFound);

}

void TableManipulationTest::getDef_data(){
	QTest::addColumn<QString>("text");
	QTest::addColumn<int>("row");
	QTest::addColumn<int>("col");
	QTest::addColumn<QString>("def_soll");

	//-------------cursor without selection--------------
	QTest::newRow("cols 2")
		<< "\\begin{tabular}{ll}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 0
		<< "ll";

	QTest::newRow("cols 4")
		<< "\\begin{tabular}{|l|l|cc}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 2 << 0
		<< "|l|l|cc";

    QTest::newRow("cols 4 tnl")
        << "\\begin{tabular}{|l|l|cc}\na&b\\tabularnewline\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
        << 2 << 0
        << "|l|l|cc";

	QTest::newRow("cols 0")
		<< "\\begin{tabular}{}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 0
		<< "";

	QTest::newRow("spaced in definition")
		<< "\\begin{tabular}{l l c}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 0
		<< "l l c";

	QTest::newRow("p")
		<< "\\begin{tabular}{llp{3cm}}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 0
		<< "llp{3cm}";

	QTest::newRow("tabularx")
		<< "\\begin{tabularx}{\\linewidth}{llm{3cm}}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabularx}\n"
		<< 1 << 0
		<< "llm{3cm}";

	QTest::newRow("col commands (array)")
		<< "\\begin{tabular}{>{\\bfseries}ll}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 1 << 0
		<< ">{\\bfseries}ll";

	QTest::newRow("no Table")
		<< "test\na&b\\\\\nc&d\\\\\ne&f\\\\\ntest\n"
		<< 1 << 0
		<< "";

	QTest::newRow("separators")
		<< "\\begin{tabular}{|l|l|@{ll}cc}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 2 << 0
		<< "|l|l|@{ll}cc";

	QTest::newRow("multipliers")
        << "\\begin{tabular}{|l|l|@{ll}c*{2}{lc}}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
		<< 2 << 0
        << "|l|l|@{ll}c*{2}{lc}";
    QTest::newRow("unrelated argument")
        << "\\begin{tabular}{|l|l|@{ll}c*{2}{lc}}{abc}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
        << 2 << 0
        << "|l|l|@{ll}c*{2}{lc}";
    QTest::newRow("colspec")
        << "\\begin{tblr}{colspec={ll}}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
        << 2 << 0
        << "ll";
    QTest::newRow("colspec2")
        << "\\begin{tblr}{width=0.8\\linewidth,colspec={|X[2,l]|X[3,l]|}}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
        << 2 << 0
        << "|X[2,l]|X[3,l]|";
    QTest::newRow("colspec3")
        << "\\begin{tblr}{width=0.8\\linewidth,colspec={|X[2,l]|X[3,l]|}}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
        << 2 << 0
        << "|X[2,l]|X[3,l]|";
    QTest::newRow("colspec in multiline")
        << "\\begin{tblr}{\n\tcolspec={ll}\n}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
        << 2 << 0
        << "ll";
    QTest::newRow("colspec in multiline2")
        << "\\begin{tblr}{\nabc,\n\tcolspec={ll}\n}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
        << 2 << 0
        << "ll";
    QTest::newRow("colspec in multiline3")
        << "\\begin{tblr}{\nabc,\n\tcolspec={ll},width={2cm}\n}\na&b\\\\\nc&d\\\\\ne&f\\\\\n\\end{tabular}\n"
        << 2 << 0
        << "ll";
}
void TableManipulationTest::getDef(){
	QFETCH(QString, text);
	QFETCH(int, row);
	QFETCH(int, col);
	QFETCH(QString, def_soll);

	ed->setText(text, false);
	ed->setCursorPosition(row,col);
	QDocumentCursor c(ed->cursor());
	QString def=LatexTables::getDef(c);
	QString sel_def=c.selectedText();

	QEQUAL(def,def_soll);
	QEQUAL(sel_def,def_soll);

}

void TableManipulationTest::parseTableLine_data(){
	QTest::addColumn<QString>("text");
	QTest::addColumn<int>("startCol");
	QTest::addColumn<int>("behindCol");
	QTest::addColumn<QString>("metaCommands");
	QTest::addColumn<QString>("lineText");
	QTest::addColumn<QString>("lineBreakOption");

	QTest::newRow("simple")
		<< "Row1 \\\\ Row2"
		<< 0 << 7
		<< "" << "Row1" << "";
	QTest::newRow("simple2")
		<< "Row1 \\\\ Row2"
		<< 7 << 12
		<< "" << "Row2" << "";
	QTest::newRow("simple3")
		<< "Row 1 & Col 2 \\\\ Row 2 & Col 2"
		<< 0 << 16
		<< "" << "Row 1 & Col 2" << "";
	QTest::newRow("lineBreakOption")
		<< "Row1 \\\\[1ex] Row2"
		<< 0 << 12
        << "" << "Row1" << "\\\\[1ex]";
    QTest::newRow("lineBreakOption2")
        << "Row1 \\\\* Row2"
        << 0 << 8
        << "" << "Row1" << "\\\\*";
    QTest::newRow("lineBreakOption3")
        << "Row1 \\tabularnewline Row2"
        << 0 << 20
        << "" << "Row1" << "\\tabularnewline";
    QTest::newRow("lineBreakOption4")
        << "Row1 \\tabularnewline[1ex] Row2"
        << 0 << 25
        << "" << "Row1" << "\\tabularnewline[1ex]";
	QTest::newRow("metaCommand")
		<< "\\hline Row1 \\\\ Row2"
		<< 0 << 14
		<< "\\hline" << "Row1" << "";
}

void TableManipulationTest::parseTableLine(){
	QFETCH(QString, text);
	QFETCH(int, startCol);
	QFETCH(int, behindCol);
	QFETCH(QString, metaCommands);
	QFETCH(QString, lineText);
	QFETCH(QString, lineBreakOption);

	LatexTableModel model(this);
	LatexTableLine *ltl = model.parseNextLine(text, startCol);
	if (ltl) {
		QEQUAL(startCol, behindCol);
		QEQUAL(ltl->toMetaLine(), metaCommands);
		QEQUAL(ltl->toColLine(), lineText);
		QEQUAL(ltl->toLineBreakOption(), lineBreakOption);
	}
}

#endif

