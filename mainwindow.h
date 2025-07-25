#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include <QDate>
#include <QDateTime>
#include <QFileDialog>
#include <QTextDocument>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



    inline void enableScrolling()
    {
        // Take the original central widget
        QWidget *originalCentralWidget = this->centralWidget();

        // Create a scroll area and set the original central widget inside it
        QScrollArea *scrollArea = new QScrollArea();
        scrollArea->setWidget(originalCentralWidget);
        scrollArea->setWidgetResizable(true);

        // Now set the scroll area as the new central widget
        this->setCentralWidget(scrollArea);
    }

private slots:
    void updateDownPaymentOp();
    void updateloanAmountOp();
    void updatetotalInitialInvestmentOp();
    void updatectotalOp();
    void updateptotalOp();
    void updaterentalIncomeOp();
    void updatevacancyRateOp();
    void updateGrossOperatingMonthlyIncomeOp();
    void updateannualCapexBudgetOp();
    void updatemaintananceBudgetOp();
    void updateGrossAnnualOperatingIncomeOp();
    void updatepropertyManagementOp();
    void updateGrossAnnualOperatingExpensesOp();
    void updatenetOperatingIncomeOp();
    void updateloanAmountMCOp();
    void updateannualInterestRateMCOp();
    void updatelifeloanOp();
    void updatetotalnoofpaymentsOp();
    void updatepaymentperperiodOp();
    void updateannualLoanPaymentsOp();
    void updatedownPaymentSFOp();
    void updateloanAmountSFOp();
    void updatemonthlyPaymentSFOp();
    void updatesellerFinancePaymentsOp();
    void updatedebtServiceCoverageRatioOp();
    void updatecapitalizationRateOp();
    void updateannualCashFlowOp();
    void updatemonthlyCashFlowOp();
    void updatenOI10CrARVOp();
    void updatenOI7CrARVOp();
    void updatenOIafterTax10CrARVOp();
    void updatenOIafterTax7CrARVOp();
    void updatenOI10CrMAOOp();
    void updatenOI7CrMAOOp();
    void updatenOIafterTax10CrMAOOp();
    void updatenOIafterTax7CrMAOOp();
    void updateballonAmountSFOp();
    void updatesumofpaymentsOp();
    void updateinterestcostOp();
    void updatePaymentSchedule();
    void updateprincipleReduction1Op();
    void updateappreciation1Op();
    void updatecashoncashReturnOp();
    void updateprincipleReductionOp();
    void updateappreciation();
    void updatetotalReturnOnInvestment();
    void resetAllFields();
    void updatetotalAnnualExpensesOp();
    void updateClosingCostsOp();
    void saveRevisionsToLocalStorage();
    void loadRevisionsFromLocalStorage();

private:
    void triggerDownload(); // Downloads the file in WASM
    void exportRevisionsToCSV();

private:
    Ui::MainWindow *ui;

private:
    bool isClosingCostsInPerSet = false;
    bool isContractPriceSet = false;
    bool isRenovationsSet = false;
    bool isDownPaymentPercentSet = false;
    bool isContractPriceSet_Loan = false;
    bool isRenovationsSet_Loan = false;
    bool isDownPaymentSet_Loan = false;
    bool isRenovationsSet_TII = false;
    bool isLeaseUpFeeSet_TII = false;
    bool isDownPaymentSet_TII = false;
    bool isLoanAmountSet_TII = false;
    bool isLoanPointsSet_TII = false;
    bool isVacancyRatePercentSet = false;
    bool isOtherIncomeSet = false;
    bool isAnnualCapexPercentSet = false;
    bool isMaintananceBudgetPercentSet = false;
    bool isPropertyManagementPercentSet = false;
    bool isTotalAnnualExpensesOpSet = false;
    bool isInterestRatePercentSet = false;
    bool isAmortizationSet = false;
    bool isNumberOfPaymentsPerYearSet = false;
    bool isDownPaymentPercentSFSet = false;
    bool isPropertyPriceSet = false;
    bool isInterestRateInPerSFSet = false;
    bool isLoanTermInMonthsSet = false;
    bool isCapRatePercentSet = false;
    bool isCapRateAssetTypePercentSet = false;
    bool isBallonDueInMonthsSet = false;
    bool isAnnualAppreciationRateSet = false;
    bool isGrossOperatingMonthlyIncomeSet = false;
    bool isGrossAnnualOperatingIncomeSet = false;
    bool isGrossAnnualOperatingExpensesSet = false;
    bool isPaymentPerPeriodSet = false;
    bool isDownPaymentSFSet = false;
    bool isMonthlyPaymentSFSet = false;
    bool isNetOperatingIncomeSet = false;
    bool isAnnualLoanPaymentsSet = false;
    bool isGrossAnnualOISet = false;
    bool isGrossAnnualOESet = false;
    bool isSellerFinancePaymentsSet = false;
    bool isAnnualCashFlowSet = false;
    bool isTotalNumberOfPaymentsSet = false;
    bool isSumOfPaymentsSet = false;
    bool isLoanAmountMCSet = false;
    bool isTotalInitialInvestmentSet = false;
    bool isPrincipleReduction1Set = false;
    bool isAppreciation1Set = false;
    bool isNOI10ARVSet = false;
    bool isNOI7ARVSet = false;
    bool isNOIAfterTax10CrARVSet = false;
    bool isNOIAfterTax7CrARVSet = false;
    bool isElectricGasSet = false;
    bool isWaterSewerSet = false;
    bool isGarbageSet = false;
    bool isHoaDuesSet = false;
    bool isPMISet = false;
    bool isPropertyTaxesSet = false;
    bool isInsuranceSet = false;
    bool isCapexSet = false;
    bool isMaintenanceSet = false;
    bool isPropertyManagementSet = false;

private:
    double precisePaymentPerPeriod;
    double preciseMonthlyPaymentSF;
    QDate loanEndDate;

private:
    void saveRevision();
    void deleteSelectedRevision();

};
#endif // MAINWINDOW_H
