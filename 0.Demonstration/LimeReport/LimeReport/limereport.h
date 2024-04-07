#ifndef LIMEREPORT_H
#define LIMEREPORT_H

#include <QWidget>
#include "lib/include/lrreportengine.h"

class LmReport : QObject
{
    Q_OBJECT
public:
    LmReport(QWidget* parent = nullptr);
    ~LmReport();

    void LoadDesignReport();
    void PreviewReport();

private:
    LimeReport::ReportEngine *m_report;


};

#endif // LIMEREPORT_H
