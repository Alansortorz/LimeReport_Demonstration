#include "limereport.h"

LmReport::LmReport(QWidget* parent)
{
    m_report = new LimeReport::ReportEngine(this);
}

LmReport::~LmReport()
{

}

void LmReport::LoadDesignReport()
{
    m_report->designReport();
}
