#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QScrollBar>
#include <cmath>
#include <QDateTime>
#include <QFileDialog>
#include <QTextDocument>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <emscripten.h>
#include <emscripten/bind.h>


// Session Persistence using javaScript
// Stores and loads on local systems and local browsers only
// Function to save data to localStorage
EM_JS(void, js_saveToLocalStorage, (const char* key, const char* value), {
    localStorage.setItem(UTF8ToString(key), UTF8ToString(value));
});

// Function to load data from localStorage
EM_JS(char*, js_loadFromLocalStorage, (const char* key), {
    var value = localStorage.getItem(UTF8ToString(key));
    if (value === null) return null;

    var lengthBytes = lengthBytesUTF8(value) + 1;
    var stringOnWasmHeap = _malloc(lengthBytes);
    stringToUTF8(value, stringOnWasmHeap, lengthBytes);
    return stringOnWasmHeap;
});

// Function to free memory allocated in JavaScript
EM_JS(void, js_free, (char* ptr), {
    _free(ptr);
});

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    enableScrolling();




    ui->centralwidget->updateGeometry();
    ui->centralwidget->adjustSize();
    //qApp->setStyleSheet("QWidget { min-width: 1024px; }");


    // updateDownPaymentOp
    connect(ui->doubleSpinBox_contractPrice, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double){
                isContractPriceSet = true;
                updateDownPaymentOp();
            });

    connect(ui->doubleSpinBox_renovations, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double){
                isRenovationsSet = true;
                updateDownPaymentOp();
            });

    connect(ui->doubleSpinBox_downpaymentInper, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double){
                isDownPaymentPercentSet = true;
                updateDownPaymentOp();
            });

    // updateClosingCostsOp
    connect(ui->doubleSpinBox_contractPrice, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isContractPriceSet = true;
                updateClosingCostsOp();
            });

    connect(ui->doubleSpinBox_closingcostsInper, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isClosingCostsInPerSet = true;
                updateClosingCostsOp();
            });

    // updateloanAmountOp
    connect(ui->doubleSpinBox_contractPrice, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isContractPriceSet_Loan = true;
                updateloanAmountOp();
            });

    connect(ui->doubleSpinBox_renovations, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isRenovationsSet_Loan = true;
                updateloanAmountOp();
            });

    connect(ui->doubleSpinBox_downPaymentOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isDownPaymentSet_Loan = true;
                updateloanAmountOp();
            });

    connect(ui->comboBox_renovations, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::updateloanAmountOp);


    // updatetotalInitialInvestmentOp
    connect(ui->doubleSpinBox_renovations, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isRenovationsSet_TII = true;
                updatetotalInitialInvestmentOp();
            });

    connect(ui->doubleSpinBox_leaseupfees, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isLeaseUpFeeSet_TII = true;
                updatetotalInitialInvestmentOp();
            });

    connect(ui->doubleSpinBox_downPaymentOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isDownPaymentSet_TII = true;
                updatetotalInitialInvestmentOp();
            });



    connect(ui->doubleSpinBox_loanAmountOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isLoanAmountSet_TII = true;
                updatetotalInitialInvestmentOp();
            });

    connect(ui->doubleSpinBox_loanPointsInper, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isLoanPointsSet_TII = true;
                updatetotalInitialInvestmentOp();
            });

    connect(ui->comboBox_renovations, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::updatetotalInitialInvestmentOp);


    // updatectotalOp
    connect(ui->doubleSpinBox_cu1, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updatectotalOp);
    connect(ui->doubleSpinBox_cu2, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updatectotalOp);
    connect(ui->doubleSpinBox_cu3, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updatectotalOp);
    connect(ui->doubleSpinBox_cu4, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updatectotalOp);
    connect(ui->doubleSpinBox_cu5, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updatectotalOp);
    connect(ui->doubleSpinBox_cu6, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updatectotalOp);
    connect(ui->doubleSpinBox_cu7, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updatectotalOp);
    connect(ui->doubleSpinBox_cu8, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updatectotalOp);
    connect(ui->doubleSpinBox_cu9, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updatectotalOp);
    connect(ui->doubleSpinBox_cu10, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updatectotalOp);
    connect(ui->doubleSpinBox_cu11, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updatectotalOp);
    connect(ui->doubleSpinBox_cu12, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updatectotalOp);
    connect(ui->doubleSpinBox_cu13, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updatectotalOp);
    connect(ui->doubleSpinBox_cu14, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updatectotalOp);

    // updateptotalOp
    connect(ui->doubleSpinBox_pu1, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updateptotalOp);
    connect(ui->doubleSpinBox_pu2, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updateptotalOp);
    connect(ui->doubleSpinBox_pu3, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updateptotalOp);
    connect(ui->doubleSpinBox_pu4, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updateptotalOp);
    connect(ui->doubleSpinBox_pu5, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updateptotalOp);
    connect(ui->doubleSpinBox_pu6, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updateptotalOp);
    connect(ui->doubleSpinBox_pu7, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updateptotalOp);
    connect(ui->doubleSpinBox_pu8, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updateptotalOp);
    connect(ui->doubleSpinBox_pu9, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updateptotalOp);
    connect(ui->doubleSpinBox_pu10, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updateptotalOp);
    connect(ui->doubleSpinBox_pu11, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updateptotalOp);
    connect(ui->doubleSpinBox_pu12, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updateptotalOp);
    connect(ui->doubleSpinBox_pu13, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updateptotalOp);
    connect(ui->doubleSpinBox_pu14, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updateptotalOp);

    // updaterentalIncomeOp
    // When the rental income type is changed
    connect(ui->comboBox_rentalIncome, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::updaterentalIncomeOp);

    // When current rents total changes
    connect(ui->doubleSpinBox_ctotalOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::updaterentalIncomeOp);

    // When proforma rents total changes
    connect(ui->doubleSpinBox_ptotalOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::updaterentalIncomeOp);

    // updatevacancyRateOp
    connect(ui->doubleSpinBox_rentalIncomeOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::updatevacancyRateOp);

    connect(ui->doubleSpinBox_vacancyRateInper, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isVacancyRatePercentSet = true;
                updatevacancyRateOp();
            });

    // updateGrossOperatingMonthlyIncomeOp
    connect(ui->doubleSpinBox_rentalIncomeOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::updateGrossOperatingMonthlyIncomeOp);

    connect(ui->doubleSpinBox_otherIncome, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isOtherIncomeSet = true;
                updateGrossOperatingMonthlyIncomeOp();
            });

    connect(ui->doubleSpinBox_vacancyRateOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::updateGrossOperatingMonthlyIncomeOp);

    // updateannualCapexBudgetOp
    connect(ui->doubleSpinBox_annualCapexbudgetInper, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isAnnualCapexPercentSet = true;
                updateannualCapexBudgetOp();
            });

    connect(ui->doubleSpinBox_rentalIncomeOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::updateannualCapexBudgetOp);

    // updatemaintananceBudgetOp
    connect(ui->doubleSpinBox_maintananceBudgetInper, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isMaintananceBudgetPercentSet = true;
                updatemaintananceBudgetOp();
            });

    connect(ui->doubleSpinBox_rentalIncomeOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::updatemaintananceBudgetOp);

    // updateGrossAnnualOperatingIncomeOp
    connect(ui->doubleSpinBox_gOMIOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isGrossOperatingMonthlyIncomeSet = true;
                updateGrossAnnualOperatingIncomeOp();
            });

    // updatepropertyManagementOp
    connect(ui->doubleSpinBox_propertyManagementInper, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isPropertyManagementPercentSet = true;
                updatepropertyManagementOp();
            });

    connect(ui->doubleSpinBox_gAOIOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::updatepropertyManagementOp);

    // updateGrossAnnualOperatingExpensesOp
    connect(ui->doubleSpinBox_totalAnnualExpensesOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isTotalAnnualExpensesOpSet = true;
                updateGrossAnnualOperatingExpensesOp();
            });

    // updatenetOperatingIncomeOp
    connect(ui->doubleSpinBox_gAOIOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isGrossAnnualOperatingIncomeSet = true;
                updatenetOperatingIncomeOp();
            });

    connect(ui->doubleSpinBox_gAOEOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isGrossAnnualOperatingExpensesSet = true;
                updatenetOperatingIncomeOp();
            });

    // updateloanAmountMCOp
    connect(ui->doubleSpinBox_loanAmountOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::updateloanAmountMCOp);

    // updateannualInterestRateMCOp
    connect(ui->doubleSpinBox_interestRateInper, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isInterestRatePercentSet = true;
                updateannualInterestRateMCOp();
            });

    // updatelifeloanOp
    connect(ui->spinBox_amortization, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [=](int) {
                isAmortizationSet = true;
                updatelifeloanOp();
            });

    // updatetotalnoofpaymentsOp
    connect(ui->spinBox_lifeloanOp, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MainWindow::updatetotalnoofpaymentsOp);

    connect(ui->spinBox_noofpaymentsperyear, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [=](int) {
                isNumberOfPaymentsPerYearSet = true;
                updatetotalnoofpaymentsOp();
            });

    //updatetotalAnnualExpensesOp
    connect(ui->doubleSpinBox_electricGas, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isElectricGasSet = true;
                updatetotalAnnualExpensesOp();
            });

    connect(ui->doubleSpinBox_waterSewer, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isWaterSewerSet = true;
                updatetotalAnnualExpensesOp();
            });

    connect(ui->doubleSpinBox_garbage, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isGarbageSet = true;
                updatetotalAnnualExpensesOp();
            });

    connect(ui->doubleSpinBox_hoaDues, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isHoaDuesSet = true;
                updatetotalAnnualExpensesOp();
            });

    connect(ui->doubleSpinBox_pmi, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isPMISet = true;
                updatetotalAnnualExpensesOp();
            });

    connect(ui->doubleSpinBox_propertyTaxes, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isPropertyTaxesSet = true;
                updatetotalAnnualExpensesOp();
            });

    connect(ui->doubleSpinBox_insurance, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isInsuranceSet = true;
                updatetotalAnnualExpensesOp();
            });

    connect(ui->doubleSpinBox_annualCapexBudgetOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isCapexSet = true;
                updatetotalAnnualExpensesOp();
            });

    connect(ui->doubleSpinBox_maintananceBudgetOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isMaintenanceSet = true;
                updatetotalAnnualExpensesOp();
            });

    connect(ui->doubleSpinBox_propertyManagementOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isPropertyManagementSet = true;
                updatetotalAnnualExpensesOp();
            });

    // updatepaymentperperiodOp
    connect(ui->doubleSpinBox_loanAmountMCOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::updatepaymentperperiodOp);

    connect(ui->doubleSpinBox_annualInterestRateMCOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::updatepaymentperperiodOp);

    connect(ui->spinBox_noofpaymentsperyear, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MainWindow::updatepaymentperperiodOp);

    connect(ui->spinBox_totalnoofpaymentsOp, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MainWindow::updatepaymentperperiodOp);

    // updateannualLoanPaymentsOp
    connect(ui->doubleSpinBox_paymentperperiodOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isPaymentPerPeriodSet = true;
                updateannualLoanPaymentsOp();
            });

    // updatedownPaymentSFOp
    connect(ui->doubleSpinBox_downPaymentInper, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isDownPaymentPercentSFSet = true;
                updatedownPaymentSFOp();
            });

    connect(ui->doubleSpinBox_propertyPrice, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isPropertyPriceSet = true;
                updatedownPaymentSFOp();
            });

    // updateloanAmountSFOp
    connect(ui->doubleSpinBox_propertyPrice, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isPropertyPriceSet = true;
                updateloanAmountSFOp();
            });

    connect(ui->doubleSpinBox_downPaymentSFOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isDownPaymentSFSet = true;
                updateloanAmountSFOp();
            });

    // updatemonthlyPaymentSFOp
    connect(ui->doubleSpinBox_loanAmountSFOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::updatemonthlyPaymentSFOp);

    connect(ui->doubleSpinBox_interestRateInper_2, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isInterestRateInPerSFSet = true;
                updatemonthlyPaymentSFOp();
            });

    connect(ui->spinBox_loanTerm, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [=](int) {
                isLoanTermInMonthsSet = true;
                updatemonthlyPaymentSFOp();
            });

    // updatesellerFinancePaymentsOp
    connect(ui->doubleSpinBox_monthlyPaymentSFOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isMonthlyPaymentSFSet = true;
                updatesellerFinancePaymentsOp();
            });
    // updatedebtServiceCoverageRatioOp
    connect(ui->doubleSpinBox_nOIOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isNetOperatingIncomeSet = true;
                updatedebtServiceCoverageRatioOp();
            });

    connect(ui->doubleSpinBox_aLPOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isAnnualLoanPaymentsSet = true;
                updatedebtServiceCoverageRatioOp();
            });

    // updatecapitalizationRateOp
    connect(ui->doubleSpinBox_nOIOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::updatecapitalizationRateOp);

    connect(ui->doubleSpinBox_contractPrice, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isContractPriceSet = true;
                updatecapitalizationRateOp();
            });
    // updateannualCashFlowOp
    connect(ui->doubleSpinBox_gAOIOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isGrossAnnualOISet = true;
                updateannualCashFlowOp();
            });

    connect(ui->doubleSpinBox_gAOEOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isGrossAnnualOESet = true;
                updateannualCashFlowOp();
            });

    connect(ui->doubleSpinBox_aLPOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isAnnualLoanPaymentsSet = true;
                updateannualCashFlowOp();
            });

    connect(ui->doubleSpinBox_sFPOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isSellerFinancePaymentsSet = true;
                updateannualCashFlowOp();
            });

    // updatemonthlyCashFlowOp
    connect(ui->doubleSpinBox_aCFOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isAnnualCashFlowSet = true;
                updatemonthlyCashFlowOp();
            });

    // updatenOI10CrARVOp
    connect(ui->doubleSpinBox_gAOIOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::updatenOI10CrARVOp);

    connect(ui->doubleSpinBox_capRateInper, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isCapRatePercentSet = true;
                updatenOI10CrARVOp();
            });

    // updatenOI7CrARVOp
    connect(ui->doubleSpinBox_gAOIOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::updatenOI7CrARVOp);

    connect(ui->doubleSpinBox_capRateCityAssetTypeInper, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isCapRateAssetTypePercentSet = true;
                updatenOI7CrARVOp();
            });

    // updatenOIafterTax10CrARVOp
    connect(ui->doubleSpinBox_nOIOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::updatenOIafterTax10CrARVOp);

    connect(ui->doubleSpinBox_capRateInper, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isCapRatePercentSet = true;
                updatenOIafterTax10CrARVOp();
            });

    // updatenOIafterTax7CrARVOp
    connect(ui->doubleSpinBox_nOIOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::updatenOIafterTax7CrARVOp);

    connect(ui->doubleSpinBox_capRateCityAssetTypeInper, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isCapRateAssetTypePercentSet = true;
                updatenOIafterTax7CrARVOp();
            });

    // updatenOI10CrMAOOp
    connect(ui->doubleSpinBox_nOI10CrARVOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isNOI10ARVSet = true;
                updatenOI10CrMAOOp();
            });

    connect(ui->doubleSpinBox_renovations, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isRenovationsSet = true;
                updatenOI10CrMAOOp();
            });

    // updatenOI7CrMAOOp
    connect(ui->doubleSpinBox_nOI7CrARVOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isNOI7ARVSet = true;
                updatenOI7CrMAOOp();
            });

    connect(ui->doubleSpinBox_renovations, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isRenovationsSet = true;
                updatenOI7CrMAOOp();
            });

    // updatenOIafterTax10CrMAOOp
    connect(ui->doubleSpinBox_nOIafterTax10CrARVOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isNOIAfterTax10CrARVSet = true;
                updatenOIafterTax10CrMAOOp();
            });

    connect(ui->doubleSpinBox_renovations, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isRenovationsSet = true;
                updatenOIafterTax10CrMAOOp();
            });

    // updatenOIafterTax7CrMAOOp
    connect(ui->doubleSpinBox_nOIafterTax7CrARVOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isNOIAfterTax7CrARVSet = true;
                updatenOIafterTax7CrMAOOp();
            });
    connect(ui->doubleSpinBox_renovations, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isRenovationsSet = true;
                updatenOIafterTax7CrMAOOp();
            });

    // updateballonAmountSFOp
    connect(ui->doubleSpinBox_loanAmountSFOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::updateballonAmountSFOp);

    connect(ui->doubleSpinBox_interestRateInper_2, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::updateballonAmountSFOp);

    connect(ui->doubleSpinBox_monthlyPaymentSFOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::updateballonAmountSFOp);

    connect(ui->comboBox_ballonPayment, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::updateballonAmountSFOp);

    connect(ui->spinBox_ballonDue, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [=](int) {
                isBallonDueInMonthsSet = true;
                updateballonAmountSFOp();
            });

    // updatesumofpaymentsOp
    connect(ui->doubleSpinBox_paymentperperiodOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isPaymentPerPeriodSet = true;
                updatesumofpaymentsOp();
            });

    connect(ui->spinBox_totalnoofpaymentsOp, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [=](int) {
                isTotalNumberOfPaymentsSet = true;
                updatesumofpaymentsOp();
            });

    // updateinterestcostOp
    connect(ui->doubleSpinBox_sumofpaymentsOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isSumOfPaymentsSet = true;
                updateinterestcostOp();
            });

    connect(ui->doubleSpinBox_loanAmountMCOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isLoanAmountMCSet = true;
                updateinterestcostOp();
            });
    // updatePaymentSchedule
    connect(ui->pushButton_generate, &QPushButton::clicked,
            this, &MainWindow::updatePaymentSchedule);

    ui->tableWidget_schedule->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);



    // updateprincipleReduction1Op
    connect(ui->pushButton_generate, &QPushButton::clicked,
            this, &MainWindow::updateprincipleReduction1Op);

    // updateappreciation1Op
    connect(ui->doubleSpinBox_contractPrice, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isContractPriceSet = true;
                updateappreciation1Op();
            });

    connect(ui->doubleSpinBox_annualARInper, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isAnnualAppreciationRateSet = true;
                updateappreciation1Op();
            });

    // updatecashoncashReturnOp
    connect(ui->doubleSpinBox_aCFOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isAnnualCashFlowSet = true;
                updatecashoncashReturnOp();
            });

    connect(ui->doubleSpinBox_totalInitialInvestmentOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isTotalInitialInvestmentSet = true;
                updatecashoncashReturnOp();
            });
    // updateprincipleReductionOp
    connect(ui->doubleSpinBox_aCFOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isAnnualCashFlowSet = true;
                updateprincipleReductionOp();
            });

    connect(ui->doubleSpinBox_principleReduction1Op, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isPrincipleReduction1Set = true;
                updateprincipleReductionOp();
            });

    connect(ui->doubleSpinBox_totalInitialInvestmentOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isTotalInitialInvestmentSet = true;
                updateprincipleReductionOp();
            });

    // updateappreciation
    connect(ui->doubleSpinBox_appreciation1Op, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isAppreciation1Set = true;
                updateappreciation();
            });

    connect(ui->doubleSpinBox_aCFOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isAnnualCashFlowSet = true;
                updateappreciation();
            });

    connect(ui->doubleSpinBox_totalInitialInvestmentOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isTotalInitialInvestmentSet = true;
                updateappreciation();
            });

    // updatetotalReturnOnInvestment
    connect(ui->doubleSpinBox_aCFOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isAnnualCashFlowSet = true;
                updatetotalReturnOnInvestment();
            });

    connect(ui->doubleSpinBox_principleReduction1Op, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isPrincipleReduction1Set = true;
                updatetotalReturnOnInvestment();
            });

    connect(ui->doubleSpinBox_appreciation1Op, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isAppreciation1Set = true;
                updatetotalReturnOnInvestment();
            });

    connect(ui->doubleSpinBox_totalInitialInvestmentOp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [=](double) {
                isTotalInitialInvestmentSet = true;
                updatetotalReturnOnInvestment();
            });


    QGridLayout *layout = qobject_cast<QGridLayout *>(ui->tab_3->layout());

    if (layout) {
        // Row 0: group boxes side by side
        layout->addWidget(ui->groupBox, 0, 0);
        layout->addWidget(ui->groupBox_mcOp, 0, 1);

        // Row 1: center the button across both columns
        QHBoxLayout *buttonLayout = new QHBoxLayout();
        buttonLayout->addStretch();
        buttonLayout->addWidget(ui->pushButton_generate);
        buttonLayout->addStretch();

        layout->addLayout(buttonLayout, 1, 0, 1, 2); // row 1, span 2 columns

        // Row 2: table widget spans both columns (full width)
        layout->addWidget(ui->tableWidget_schedule, 2, 0, 1, 2);

        // Optional: make the table stretch to take extra vertical space
        layout->setRowStretch(2, 1);

        // ui->frame_Header->layout()->setContentsMargins(10, 5, 10, 5);

    }

    // pushButton_resetALL

    ui->pushButton_resetALL->setMaximumWidth(250);  // Optional width

    // pushBotton_saveRevisions

    ui->pushButton_saveRevisions->setMaximumWidth(250);

    ui->pushButton_deleteRevisions->setMaximumWidth(250);

    ui->pushButton_download->setMaximumWidth(200);

    connect(ui->pushButton_resetALL, &QPushButton::clicked, this, &MainWindow::resetAllFields);
    ui->dateEdit_myDate->setDisplayFormat("dd-MM-yyyy");
    ui->dateEdit_myDate->setMaximumWidth(130);
    ui->spinBox_noofpaymentsperyear->setMaximumWidth(130);
    ui->textEdit_pAddress->setMinimumHeight(60);
    ui->textEdit_pAddress->setMaximumHeight(60);
    ui->textEdit_pNotes->setMinimumHeight(60);
    ui->textEdit_pNotes->setMaximumHeight(60);
    ui->textEdit_pNotes->setMinimumWidth(840);
    ui->textEdit_pNotes->setMaximumWidth(840);

    ui->tableWidget_revisions->setColumnCount(2);
    QStringList headers = {"Time Stamp", "Summary"};
    ui->tableWidget_schedule->setHorizontalHeaderLabels(headers);
    ui->tableWidget_revisions->setColumnWidth(0, 450); // Time Stamp column
    ui->tableWidget_revisions->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_revisions->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->tableWidget_revisions->setFocusPolicy(Qt::WheelFocus);
    ui->tableWidget_revisions->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    connect(ui->pushButton_saveRevisions, &QPushButton::clicked, this, &MainWindow::saveRevision);
    connect(ui->pushButton_deleteRevisions, &QPushButton::clicked, this, &MainWindow::deleteSelectedRevision);
    connect(ui->pushButton_download, &QPushButton::clicked, this, &MainWindow::triggerDownload);


    loadRevisionsFromLocalStorage();
}

// Storing the revisions in LocalStorage
void MainWindow::saveRevisionsToLocalStorage() {
    QJsonArray revisionsArray;

    // Convert all revisions to JSON
    for (int i = 0; i < ui->tableWidget_revisions->rowCount(); ++i) {
        QJsonObject revision;
        revision["timestamp"] = ui->tableWidget_revisions->item(i, 0)->text();

        QLabel* label = qobject_cast<QLabel*>(ui->tableWidget_revisions->cellWidget(i, 1));
        if (label) {
            revision["summary"] = label->text();
        }

        revisionsArray.append(revision);
    }

    // Store in localStorage
    QByteArray jsonData = QJsonDocument(revisionsArray).toJson();
    js_saveToLocalStorage("dealAnalyzerRevisions", jsonData.constData());
}

//Loading the revisions from LocalStorage
void MainWindow::loadRevisionsFromLocalStorage() {
    char* data = js_loadFromLocalStorage("dealAnalyzerRevisions");
    if (data) {
        QByteArray jsonData(data);
        js_free(data); // Free the allocated memory

        QJsonDocument doc = QJsonDocument::fromJson(jsonData);
        if (!doc.isNull()) {
            QJsonArray revisionsArray = doc.array();

            ui->tableWidget_revisions->setRowCount(0); // Clear existing

            for (const QJsonValue& value : revisionsArray) {
                QJsonObject revision = value.toObject();
                int newRow = ui->tableWidget_revisions->rowCount();
                ui->tableWidget_revisions->insertRow(newRow);

                QString timestamp = revision["timestamp"].toString();
                QString summary = revision["summary"].toString();

                QLabel *summaryLabel = new QLabel();
                summaryLabel->setText(summary);
                summaryLabel->setTextFormat(Qt::RichText);
                summaryLabel->setWordWrap(true);
                summaryLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
                summaryLabel->setMargin(5);

                ui->tableWidget_revisions->setItem(newRow, 0, new QTableWidgetItem(timestamp));
                ui->tableWidget_revisions->setCellWidget(newRow, 1, summaryLabel);
            }

            ui->tableWidget_revisions->resizeRowsToContents();
        }
    }
}

// To download revisions in CSV format
void MainWindow::triggerDownload()
{
    if (ui->tableWidget_revisions->rowCount() == 0)
        return;

    exportRevisionsToCSV();
}

void MainWindow::exportRevisionsToCSV()
{
    QString csv;
    QTextStream stream(&csv);

    // Write headers
    QStringList headers;
    for (int col = 0; col < ui->tableWidget_revisions->columnCount(); ++col) {
        QTableWidgetItem *headerItem = ui->tableWidget_revisions->horizontalHeaderItem(col);
        headers << "\"" + (headerItem ? headerItem->text().replace("\"", "\"\"") : QString("Column_%1").arg(col)) + "\"";
    }
    stream << headers.join(",") << "\n";

    // Write rows
    for (int row = 0; row < ui->tableWidget_revisions->rowCount(); ++row) {
        QStringList rowData;
        for (int col = 0; col < ui->tableWidget_revisions->columnCount(); ++col) {
            QString cellText;

            // Handle Summary column (cellWidget QLabel with HTML)
            if (col == 1) {
                QWidget *cellWidget = ui->tableWidget_revisions->cellWidget(row, col);
                if (QLabel *label = qobject_cast<QLabel *>(cellWidget)) {
                    QTextDocument doc;
                    doc.setHtml(label->text());
                    cellText = doc.toPlainText();
                } else {
                    cellText = "";
                }
            } else {
                QTableWidgetItem *item = ui->tableWidget_revisions->item(row, col);
                if (item) {
                    cellText = item->text();
                } else {
                    cellText = "";
                }
            }

            // Properly escape quotes and preserve newlines
            cellText.replace("\"", "\"\"");  // Escape quotes for CSV
            cellText = "\"" + cellText + "\"";  // Wrap field in quotes
            rowData << cellText;

        }
        stream << rowData.join(",") << "\n";
    }

    // Generate filename
    QString filename = QString("RevisionTable_%1.csv")
                           .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss"));

#ifdef Q_OS_WASM
    QFileDialog::saveFileContent(csv.toUtf8(), filename);
#endif
}

// To save revisions
void MainWindow::saveRevision()
{
    int newRow = ui->tableWidget_revisions->rowCount();
    ui->tableWidget_revisions->insertRow(newRow);
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    double aP = ui->doubleSpinBox_askingPrice->value();
    double cP = ui->doubleSpinBox_contractPrice->value();
    QString rType = ui->comboBox_renovations->currentText();
    double re = ui->doubleSpinBox_renovations->value();
    double luf = ui->doubleSpinBox_leaseupfees->value();
    double dppda = ui->doubleSpinBox_downpaymentInper->value();
    double dpda = ui->doubleSpinBox_downPaymentOp->value();
    double cCP = ui->doubleSpinBox_closingcostsInper->value();
    double cC = ui->doubleSpinBox_closingCostsOp->value();
    double tiv = ui->doubleSpinBox_totalInitialInvestmentOp->value();
    QString rIType = ui->comboBox_rentalIncome->currentText();
    double rI = ui->doubleSpinBox_rentalIncomeOp->value();
    double oI = ui->doubleSpinBox_otherIncome->value();
    double vRP = ui->doubleSpinBox_vacancyRateInper->value();
    double vR = ui->doubleSpinBox_vacancyRateOp->value();
    double gOMI = ui->doubleSpinBox_gOMIOp->value();
    double eG = ui->doubleSpinBox_electricGas->value();
    double wS = ui->doubleSpinBox_waterSewer->value();
    double ga = ui->doubleSpinBox_garbage->value();
    double hoa = ui->doubleSpinBox_hoaDues->value();
    double pmi = ui->doubleSpinBox_pmi->value();
    double pT = ui->doubleSpinBox_propertyTaxes->value();
    double in = ui->doubleSpinBox_insurance->value();
    double aCBP = ui->doubleSpinBox_annualCapexbudgetInper->value();
    double aCB = ui->doubleSpinBox_annualCapexBudgetOp->value();
    double mBP = ui->doubleSpinBox_maintananceBudgetInper->value();
    double mB = ui->doubleSpinBox_maintananceBudgetOp->value();
    double pMP = ui->doubleSpinBox_propertyManagementInper->value();
    double pM = ui->doubleSpinBox_propertyManagementOp->value();
    double tAE = ui->doubleSpinBox_totalAnnualExpensesOp->value();
    double cRP = ui->doubleSpinBox_capRateInper->value();
    double cRCATP = ui->doubleSpinBox_capRateCityAssetTypeInper->value();
    double nOICR10ARV = ui->doubleSpinBox_nOI10CrARVOp->value();
    double nOICR7ARV = ui->doubleSpinBox_nOI7CrARVOp->value();
    double nOICR10ATARV = ui->doubleSpinBox_nOIafterTax10CrARVOp->value();
    double nOICR7ATARV = ui->doubleSpinBox_nOIafterTax7CrARVOp->value();
    double nOI10CRMAO = ui->doubleSpinBox_nOI10CrMAOOp->value();
    double nOI7CRMAO = ui->doubleSpinBox_nOI7CrMAOOp->value();
    double nOI10CRATMAO = ui->doubleSpinBox_nOIafterTax10CrMAOOp->value();
    double nOI7CRATMAO = ui->doubleSpinBox_nOIafterTax7CrMAOOp->value();
    double gAOI = ui->doubleSpinBox_gAOIOp->value();
    double gAOE = ui->doubleSpinBox_gAOEOp->value();
    double nOI = ui->doubleSpinBox_nOIOp->value();
    double aLP = ui->doubleSpinBox_aLPOp->value();
    double sFP = ui->doubleSpinBox_sFPOp->value();
    double dSCR = ui->doubleSpinBox_dSCROp->value();
    double capiR = ui->doubleSpinBox_capitalizationRateOp->value();
    double aCF = ui->doubleSpinBox_aCFOp->value();
    double mCF = ui->doubleSpinBox_mCFOp->value();
    double pR1 = ui->doubleSpinBox_principleReduction1Op->value();
    double a1 = ui->doubleSpinBox_appreciation1Op->value();
    double cOCR = ui->doubleSpinBox_cashoncashReturnOp->value();
    double pR = ui->doubleSpinBox_principleReductionOp->value();
    double app = ui->doubleSpinBox_appreciation->value();
    double tROI = ui->doubleSpinBox_totalROIOp->value();
    double lA = ui->doubleSpinBox_loanAmountOp->value();
    double lPP = ui->doubleSpinBox_loanPointsInper->value();
    int am = ui->spinBox_amortization->value();
    double iR = ui->doubleSpinBox_interestRateInper->value();
    double aPRP = ui->doubleSpinBox_annualARInper->value();
    double tCR = ui->doubleSpinBox_ctotalOp->value();
    double tPR = ui->doubleSpinBox_ptotalOp->value();
    QString pADD = ui->textEdit_pAddress->toPlainText();
    QString pNOT = ui->textEdit_pNotes->toPlainText();
    // SF
    double pP = ui->doubleSpinBox_propertyPrice->value();
    double dPPSF = ui->doubleSpinBox_downPaymentInper->value();
    double dPSF = ui->doubleSpinBox_downPaymentSFOp->value();
    double lASF = ui->doubleSpinBox_loanAmountSFOp->value();
    double iRSF = ui->doubleSpinBox_interestRateInper_2->value();
    int lT = ui->spinBox_loanTerm->value();
    double mPSF = ui->doubleSpinBox_monthlyPaymentSFOp->value();
    QString bType = ui->comboBox_ballonPayment->currentText();
    int bD = ui->spinBox_ballonDue->value();
    double bA = ui->doubleSpinBox_ballonAmountSFOp->value();
    // MC
    double lAMC = ui->doubleSpinBox_loanAmountMCOp->value();
    double aIR = ui->doubleSpinBox_annualInterestRateMCOp->value();
    int lL = ui->spinBox_lifeloanOp->value();
    int nPPY = ui->spinBox_noofpaymentsperyear->value();
    int tNP = ui->spinBox_totalnoofpaymentsOp->value();
    double pPP = ui->doubleSpinBox_paymentperperiodOp->value();
    double sPMC = ui->doubleSpinBox_sumofpaymentsOp->value();
    double iC = ui->doubleSpinBox_interestcostOp->value();
    QDate lSD = ui->dateEdit_myDate->date();
    QDate lED = loanEndDate;

    QString summary = QString(
                          "<div style='text-align: center;'><h3>Deal Analyzer</h3></div>"
                          "<hr style='border: none; border-top: 1px dotted #000; margin: 5px 0;'>"
                          "<b>Property Address</b>: %1<br>"
                          "<b>Property Notes</b>: %2<br>"
                          "<b>Asking Price</b>: $%3<br>"
                          "Contract Price: $%4<br>"
                          "Renovation Type: %5<br>"
                          "Renovation Estimate: $%6<br>"
                          "Lease Up Fees: $%7<br>"
                          "<b>Down Payment (%)</b>: %8%<br>"
                          "<b>Down Payment ($)</b>: $%9<br>"
                          "Closing costs (%) : %83%<br>"
                          "Closing costs ($) : $%84<br>"
                          "<b>Total Initial Investment</b>: $%10<br>"
                          "Rental Income Type: %11<br>"
                          "<b>Rental Income</b>: $%12<br>"
                          "Other Income: $%13<br>"
                          "Vacancy Rate (%): %14%<br>"
                          "Vacancy Rate ($): $%15<br>"
                          "<b>Gross Operating Monthly Income</b>: $%16<br>"
                          "Electric + Gas: $%17<br>"
                          "Water + Sewer: $%18<br>"
                          "Garbage: $%19<br>"
                          "HOA Dues (condos only): $%20<br>"
                          "PMI (if under 20% down): $%21<br>"
                          "Property Taxes: $%22<br>"
                          "Insurance: $%23<br>"
                          "Annual CapEx Budget (%): %24%  | Amount: $%25<br>"
                          "Maintenance Budget (%): %26%  | Amount: $%27<br>"
                          "Property Management (%): %28%  | Amount: $%29<br>"
                          "<b>Total Annual Expenses</b>: $%30<br>"
                          "<b>Cap Rate (%)</b>: %31%<br>"
                          "<b>Cap Rate (City/Asset Type) (%)</b>: %32%<br>"
                          "NOI 10% Cap Rate ARV: $%33<br>"
                          "NOI 7% Cap Rate ARV: $%34<br>"
                          "NOI (after taxes & insurance) / 10% Cap Rate ARV: $%35<br>"
                          "NOI (after taxes & insurance) / 7% Cap Rate ARV: $%36<br>"
                          "NOI 10% Cap Rate MAO: $%37<br>"
                          "NOI 7% Cap Rate MAO: $%38<br>"
                          "NOI (after taxes & insurance) / 10% Cap Rate MAO: $%39<br>"
                          "NOI (after taxes & insurance) / 7% Cap Rate MAO: $%40<br>"
                          "Gross Annual Operating Income: $%41<br>"
                          "Gross Annual Operating Expenses: $%42<br>"
                          "Net Operating Income: $%43<br>"
                          "Annual Loan Payments: $%44<br>"
                          "Seller Finance Payments: $%45<br>"
                          "<b>DSCR (Debt Service Coverage Ratio)</b>: %46<br>"
                          "Capitalization Rate: %47%<br>"
                          "<b>Annual Cash Flow</b>: $%48<br>"
                          "<b>Monthly Cash Flow</b>: $%49<br>"
                          "Principal Reduction In First Year: $%50<br>"
                          "Appreciation In First Year $%51<br>"
                          "<b>Cash on Cash Return</b>: %52%<br>"
                          "Total Principal Reduction: $%53<br>"
                          "Total Appreciation: $%54<br>"
                          "<b>Total Return On Investment</b>: %55%<br>"
                          "<b>Loan Amount</b>: $%56<br>"
                          "Loan Points (%): %57%<br>"
                          "Amortization (yrs): %58<br>"
                          "Interest Rate (%): %59%<br>"
                          "Annual Appreciation Rate (%): %60%<br>"
                          "Total Current Rents: $%61<br>"
                          "Total Proforma Rents: $%62<br><br>"

                          "<div style='text-align: center;'><h3>Seller Financing</h3></div>"
                          "<hr style='border: none; border-top: 1px dotted #000; margin: 5px 0;'>"
                          "<b>Property Price</b>: $%63<br>"
                          "<b>Down Payment (%)</b>: %64%<br>"
                          "<b>Down Payment ($)</b>: $%65<br>"
                          "<b>Loan Amount</b>: $%66<br>"
                          "Interest Rate: %67%<br>"
                          "Loan Term (yrs): %68<br>"
                          "<b>Monthly Payment</b>: $%69<br>"
                          "Balloon Type: %70<br>"
                          "Balloon Due (Month): %71<br>"
                          "<b>Balloon Amount</b>: $%72<br><br>"

                          "<div style='text-align: center;'><h3>Mortgage Calculator</h3></div>"
                          "<hr style='border: none; border-top: 1px dotted #000; margin: 5px 0;'>"
                          "<b>Loan Amount</b>: $%73<br>"
                          "<b>Annual Interest Rate</b>: %74%<br>"
                          "Loan Life (yrs): %75<br>"
                          "Payments/Year: %76<br>"
                          "Total Payments/Year: %77<br>"
                          "<b>Payment Per Period</b>: $%78<br>"
                          "Sum of Payments (Total Cost of Loan): $%79<br>"
                          "Interest Cost: $%80<br>"
                          "<b>Loan Starting Date: %81</b><br>"
                          "<b>Loan Ending Date</b>: %82"
                          )
                          .arg(pADD)
                          .arg(pNOT)
                          .arg(aP, 0, 'f', 2)
                          .arg(cP, 0, 'f', 2)
                          .arg(rType)
                          .arg(re, 0, 'f', 2)
                          .arg(luf, 0, 'f', 2)
                          .arg(dppda, 0, 'f', 2)
                          .arg(dpda, 0, 'f', 2)
                          .arg(tiv, 0, 'f', 2)
                          .arg(rIType)
                          .arg(rI, 0, 'f', 2)
                          .arg(oI, 0, 'f', 2)
                          .arg(vRP, 0, 'f', 2)
                          .arg(vR, 0, 'f', 2)
                          .arg(gOMI, 0, 'f', 2)
                          .arg(eG, 0, 'f', 2)
                          .arg(wS, 0, 'f', 2)
                          .arg(ga, 0, 'f', 2)
                          .arg(hoa, 0, 'f', 2)
                          .arg(pmi, 0, 'f', 2)
                          .arg(pT, 0, 'f', 2)
                          .arg(in, 0, 'f', 2)
                          .arg(aCBP, 0, 'f', 2)
                          .arg(aCB, 0, 'f', 2)
                          .arg(mBP, 0, 'f', 2)
                          .arg(mB, 0, 'f', 2)
                          .arg(pMP, 0, 'f', 2)
                          .arg(pM, 0, 'f', 2)
                          .arg(tAE, 0, 'f', 2)
                          .arg(cRP, 0, 'f', 2)
                          .arg(cRCATP, 0, 'f', 2)
                          .arg(nOICR10ARV, 0, 'f', 2)
                          .arg(nOICR7ARV, 0, 'f', 2)
                          .arg(nOICR10ATARV, 0, 'f', 2)
                          .arg(nOICR7ATARV, 0, 'f', 2)
                          .arg(nOI10CRMAO, 0, 'f', 2)
                          .arg(nOI7CRMAO, 0, 'f', 2)
                          .arg(nOI10CRATMAO, 0, 'f', 2)
                          .arg(nOI7CRATMAO, 0, 'f', 2)
                          .arg(gAOI, 0, 'f', 2)
                          .arg(gAOE, 0, 'f', 2)
                          .arg(nOI, 0, 'f', 2)
                          .arg(aLP, 0, 'f', 2)
                          .arg(sFP, 0, 'f', 2)
                          .arg(dSCR, 0, 'f', 2)
                          .arg(capiR, 0, 'f', 2)
                          .arg(aCF, 0, 'f', 2)
                          .arg(mCF, 0, 'f', 2)
                          .arg(pR1, 0, 'f', 2)
                          .arg(a1, 0, 'f', 2)
                          .arg(cOCR, 0, 'f', 2)
                          .arg(pR, 0, 'f', 2)
                          .arg(app, 0, 'f', 2)
                          .arg(tROI, 0, 'f', 2)
                          .arg(lA, 0, 'f', 2)
                          .arg(lPP, 0, 'f', 2)
                          .arg(am)
                          .arg(iR, 0, 'f', 2)
                          .arg(aPRP, 0, 'f', 2)
                          .arg(tCR, 0, 'f', 2)
                          .arg(tPR, 0, 'f', 2)
                          .arg(pP, 0, 'f', 2)
                          .arg(dPPSF, 0, 'f', 2)
                          .arg(dPSF, 0, 'f', 2)
                          .arg(lASF, 0, 'f', 2)
                          .arg(iRSF, 0, 'f', 2)
                          .arg(lT)
                          .arg(mPSF, 0, 'f', 2)
                          .arg(bType)
                          .arg(bD)
                          .arg(bA, 0, 'f', 2)
                          .arg(lAMC, 0, 'f', 2)
                          .arg(aIR, 0, 'f', 2)
                          .arg(lL)
                          .arg(nPPY)
                          .arg(tNP)
                          .arg(pPP, 0, 'f', 2)
                          .arg(sPMC, 0, 'f', 2)
                          .arg(iC, 0, 'f', 2)
                          .arg(lSD.toString("dd-MM-yyyy"))
                          .arg(lED.toString("dd-MM-yyyy"))
                          .arg(cCP, 0, 'f', 2)
                          .arg(cC, 0, 'f', 2);

    // Create QLabel with HTML support for the summary column
    QLabel *summaryLabel = new QLabel();
    summaryLabel->setText(summary);
    summaryLabel->setTextFormat(Qt::RichText);
    summaryLabel->setWordWrap(true);
    summaryLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    summaryLabel->setMargin(5);

    // summaryLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
    // summaryLabel->adjustSize();

    // // Calculate and set minimum height based on content
    // QFontMetrics fm(summaryLabel->font());
    // int textWidth = ui->tableWidget_revisions->columnWidth(1) - 10; // Account for margins
    // QRect boundingRect = fm.boundingRect(0, 0, textWidth, 0, Qt::TextWordWrap, summary);
    // summaryLabel->setMinimumHeight(boundingRect.height() + 20);

    ui->tableWidget_revisions->setItem(newRow, 0, new QTableWidgetItem(timestamp));
    ui->tableWidget_revisions->setCellWidget(newRow, 1, summaryLabel);

    // Remove the fixed row height line since we want dynamic sizing
    // ui->tableWidget_revisions->setRowHeight(newRow, 90); // Remove this line

    // Resize columns to content first
    // ui->tableWidget_revisions->resizeColumnsToContents();

    // Then resize rows to fit the content
    ui->tableWidget_revisions->resizeRowsToContents();

    saveRevisionsToLocalStorage();
    // Alternative: Set a more flexible resize mode for better display
    // ui->tableWidget_revisions->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

//To delete revisions
void MainWindow::deleteSelectedRevision()
{
    int row = ui->tableWidget_revisions->currentRow();
    if (row >= 0) {
        ui->tableWidget_revisions->removeRow(row);
        saveRevisionsToLocalStorage();
    }

}

//To reset all the values
void MainWindow::resetAllFields()
{
    ui->textEdit_pAddress->setText("");
    ui->textEdit_pNotes->setText("");
    //spin boxes
    ui->spinBox_amortization->setValue(0);
    ui->spinBox_noofpaymentsperyear->setValue(0);
    ui->spinBox_totalnoofpaymentsOp->setValue(0);
    ui->spinBox_loanTerm->setValue(0);
    ui->spinBox_ballonDue->setValue(0);
    ui->spinBox_lifeloanOp->setValue(0);
    // ComboBoxes
    ui->comboBox_renovations->setCurrentIndex(0);
    ui->comboBox_ballonPayment->setCurrentIndex(0);
    ui->comboBox_rentalIncome->setCurrentIndex(0);
    // Reset table
    ui->tableWidget_schedule->clear();
    ui->tableWidget_schedule->setRowCount(0);
    ui->tableWidget_schedule->setColumnCount(0);
    // Reset date
    ui->dateEdit_myDate->setDate(QDate::currentDate());
    //doublespinboxes
    ui->doubleSpinBox_askingPrice->setValue(0);
    ui->doubleSpinBox_contractPrice->setValue(0);
    ui->doubleSpinBox_downpaymentInper->setValue(0);
    ui->doubleSpinBox_renovations->setValue(0);
    ui->doubleSpinBox_closingCostsOp->setValue(0);
    ui->doubleSpinBox_leaseupfees->setValue(0);
    ui->doubleSpinBox_closingcostsInper->setValue(0);
    ui->doubleSpinBox_downPaymentOp->setValue(0);
    ui->doubleSpinBox_totalInitialInvestmentOp->setValue(0);
    ui->doubleSpinBox_rentalIncomeOp->setValue(0);
    ui->doubleSpinBox_otherIncome->setValue(0);
    ui->doubleSpinBox_vacancyRateInper->setValue(0);
    ui->doubleSpinBox_vacancyRateOp->setValue(0);
    ui->doubleSpinBox_gOMIOp->setValue(0);
    ui->doubleSpinBox_electricGas->setValue(0);
    ui->doubleSpinBox_waterSewer->setValue(0);
    ui->doubleSpinBox_garbage->setValue(0);
    ui->doubleSpinBox_hoaDues->setValue(0);
    ui->doubleSpinBox_pmi->setValue(0);
    ui->doubleSpinBox_propertyTaxes->setValue(0);
    ui->doubleSpinBox_insurance->setValue(0);
    ui->doubleSpinBox_annualCapexbudgetInper->setValue(0);
    ui->doubleSpinBox_annualCapexBudgetOp->setValue(0);
    ui->doubleSpinBox_maintananceBudgetInper->setValue(0);
    ui->doubleSpinBox_maintananceBudgetOp->setValue(0);
    ui->doubleSpinBox_propertyManagementInper->setValue(0);
    ui->doubleSpinBox_propertyManagementOp->setValue(0);
    ui->doubleSpinBox_totalAnnualExpensesOp->setValue(0);
    ui->doubleSpinBox_capRateInper->setValue(0);
    ui->doubleSpinBox_capRateCityAssetTypeInper->setValue(0);
    ui->doubleSpinBox_nOI10CrARVOp->setValue(0);
    ui->doubleSpinBox_nOI7CrARVOp->setValue(0);
    ui->doubleSpinBox_nOIafterTax10CrARVOp->setValue(0);
    ui->doubleSpinBox_nOIafterTax7CrARVOp->setValue(0);
    ui->doubleSpinBox_nOI10CrMAOOp->setValue(0);
    ui->doubleSpinBox_nOI7CrMAOOp->setValue(0);
    ui->doubleSpinBox_nOIafterTax10CrMAOOp->setValue(0);
    ui->doubleSpinBox_nOIafterTax7CrMAOOp->setValue(0);
    ui->doubleSpinBox_gAOIOp->setValue(0);
    ui->doubleSpinBox_gAOEOp->setValue(0);
    ui->doubleSpinBox_nOIOp->setValue(0);
    ui->doubleSpinBox_aLPOp->setValue(0);
    ui->doubleSpinBox_sFPOp->setValue(0);
    ui->doubleSpinBox_capitalizationRateOp->setValue(0);
    ui->doubleSpinBox_aCFOp->setValue(0);
    ui->doubleSpinBox_mCFOp->setValue(0);
    ui->doubleSpinBox_principleReduction1Op->setValue(0);
    ui->doubleSpinBox_appreciation1Op->setValue(0);
    ui->doubleSpinBox_cashoncashReturnOp->setValue(0);
    ui->doubleSpinBox_principleReductionOp->setValue(0);
    ui->doubleSpinBox_appreciation->setValue(0);
    ui->doubleSpinBox_totalROIOp->setValue(0);
    ui->doubleSpinBox_dSCROp->setValue(0);
    ui->doubleSpinBox_loanAmountOp->setValue(0);
    ui->doubleSpinBox_loanPointsInper->setValue(0);
    ui->doubleSpinBox_interestRateInper->setValue(0);
    ui->doubleSpinBox_annualARInper->setValue(0);
    ui->doubleSpinBox_cu1->setValue(0);
    ui->doubleSpinBox_cu2->setValue(0);
    ui->doubleSpinBox_cu3->setValue(0);
    ui->doubleSpinBox_cu4->setValue(0);
    ui->doubleSpinBox_cu5->setValue(0);
    ui->doubleSpinBox_cu6->setValue(0);
    ui->doubleSpinBox_cu7->setValue(0);
    ui->doubleSpinBox_cu8->setValue(0);
    ui->doubleSpinBox_cu9->setValue(0);
    ui->doubleSpinBox_cu10->setValue(0);
    ui->doubleSpinBox_cu11->setValue(0);
    ui->doubleSpinBox_cu12->setValue(0);
    ui->doubleSpinBox_cu13->setValue(0);
    ui->doubleSpinBox_cu14->setValue(0);
    ui->doubleSpinBox_pu1->setValue(0);
    ui->doubleSpinBox_pu2->setValue(0);
    ui->doubleSpinBox_pu3->setValue(0);
    ui->doubleSpinBox_pu4->setValue(0);
    ui->doubleSpinBox_pu5->setValue(0);
    ui->doubleSpinBox_pu6->setValue(0);
    ui->doubleSpinBox_pu7->setValue(0);
    ui->doubleSpinBox_pu8->setValue(0);
    ui->doubleSpinBox_pu9->setValue(0);
    ui->doubleSpinBox_pu10->setValue(0);
    ui->doubleSpinBox_pu11->setValue(0);
    ui->doubleSpinBox_pu12->setValue(0);
    ui->doubleSpinBox_pu13->setValue(0);
    ui->doubleSpinBox_pu14->setValue(0);
    ui->doubleSpinBox_propertyPrice->setValue(0);
    ui->doubleSpinBox_loanAmountSFOp->setValue(0);
    ui->doubleSpinBox_downPaymentInper->setValue(0);
    ui->doubleSpinBox_downPaymentSFOp->setValue(0);
    ui->doubleSpinBox_interestRateInper_2->setValue(0);
    ui->doubleSpinBox_monthlyPaymentSFOp->setValue(0);
    ui->doubleSpinBox_ballonAmountSFOp->setValue(0);
    ui->doubleSpinBox_loanAmountMCOp->setValue(0);
    ui->doubleSpinBox_annualInterestRateMCOp->setValue(0);
    ui->doubleSpinBox_paymentperperiodOp->setValue(0);
    ui->doubleSpinBox_sumofpaymentsOp->setValue(0);
    ui->doubleSpinBox_interestcostOp->setValue(0);
}

void MainWindow::updateDownPaymentOp()
{
    if (!isContractPriceSet || !isRenovationsSet || !isDownPaymentPercentSet)
        return;

    double contractPrice = ui->doubleSpinBox_contractPrice->value();
    double renovations = ui->doubleSpinBox_renovations->value();
    double downPaymentPercent = ui->doubleSpinBox_downpaymentInper->value() / 100;

    double result = (contractPrice + renovations) * downPaymentPercent;
    qDebug() << result << " :downPaymentOp";

    ui->doubleSpinBox_downPaymentOp->setValue(result);
}

void MainWindow::updateClosingCostsOp()
{
    if (!isContractPriceSet || !isClosingCostsInPerSet)
        return;
    double contractPrice = ui->doubleSpinBox_contractPrice->value();
    double closingCostsInper = ui->doubleSpinBox_closingcostsInper->value() / 100;
    double result = contractPrice * closingCostsInper;
    qDebug() << result << " :ClosingCostsOp";

    ui->doubleSpinBox_closingCostsOp->setValue(result);
}


void MainWindow::updateloanAmountOp()
{
    if (!(isContractPriceSet_Loan && isDownPaymentSet_Loan &&
          (ui->comboBox_renovations->currentText() == "" || isRenovationsSet_Loan)))
    {
        return; // Wait until all necessary inputs are set
    }

    double contractPrice = ui->doubleSpinBox_contractPrice->value();
    double renovations = ui->doubleSpinBox_renovations->value();
    double downPayment = ui->doubleSpinBox_downPaymentOp->value();
    QString renovationsType = ui->comboBox_renovations->currentText();

    double result = (renovationsType == "Cash")
                        ? contractPrice - downPayment
                        : contractPrice + renovations - downPayment;

    qDebug() << result << " :loanAmountOp";
    ui->doubleSpinBox_loanAmountOp->setValue(result);
}

void MainWindow::updatetotalInitialInvestmentOp()
{
    if (!(isLeaseUpFeeSet_TII &&
          isDownPaymentSet_TII &&
          isLoanAmountSet_TII &&
          isLoanPointsSet_TII &&
          isClosingCostsInPerSet &&
          (ui->comboBox_renovations->currentText() != "" || isRenovationsSet_TII)))
    {
        return; // Wait until all necessary inputs are set
    }

    QString renovationsType = ui->comboBox_renovations->currentText();
    double renovations = ui->doubleSpinBox_renovations->value();
    double leaseUpFee = ui->doubleSpinBox_leaseupfees->value();
    double downPayment = ui->doubleSpinBox_downPaymentOp->value();
    double loanAmount = ui->doubleSpinBox_loanAmountOp->value();
    double loanPoints = ui->doubleSpinBox_loanPointsInper->value();
    double closingCosts = ui->doubleSpinBox_closingCostsOp->value();

    double result = (renovationsType == "Cash")
                        ? (renovations + leaseUpFee + downPayment + closingCosts) + (loanAmount * (loanPoints / 100))
                        : (leaseUpFee + downPayment + closingCosts) + (loanAmount * (loanPoints / 100));

    qDebug() << result << " :totalInitialInvestmentOp";
    ui->doubleSpinBox_totalInitialInvestmentOp->setValue(result);
}

void MainWindow::updatectotalOp()
{
    double cu1 = ui->doubleSpinBox_cu1->value();
    double cu2 = ui->doubleSpinBox_cu2->value();
    double cu3 = ui->doubleSpinBox_cu3->value();
    double cu4 = ui->doubleSpinBox_cu4->value();
    double cu5 = ui->doubleSpinBox_cu5->value();
    double cu6 = ui->doubleSpinBox_cu6->value();
    double cu7 = ui->doubleSpinBox_cu7->value();
    double cu8 = ui->doubleSpinBox_cu8->value();
    double cu9 = ui->doubleSpinBox_cu9->value();
    double cu10 = ui->doubleSpinBox_cu10->value();
    double cu11 = ui->doubleSpinBox_cu11->value();
    double cu12 = ui->doubleSpinBox_cu12->value();
    double cu13 = ui->doubleSpinBox_cu13->value();
    double cu14 = ui->doubleSpinBox_cu14->value();
    double result = cu1+cu2+cu3+cu4+cu5+cu6+cu7+cu8+cu9+cu10+cu11+cu12+cu13+cu14;
    qDebug() << result << " :currentRentsTotalOp";
    ui->doubleSpinBox_ctotalOp->setValue(result);
}

void MainWindow::updateptotalOp()
{
    double pu1 = ui->doubleSpinBox_pu1->value();
    double pu2 = ui->doubleSpinBox_pu2->value();
    double pu3 = ui->doubleSpinBox_pu3->value();
    double pu4 = ui->doubleSpinBox_pu4->value();
    double pu5 = ui->doubleSpinBox_pu5->value();
    double pu6 = ui->doubleSpinBox_pu6->value();
    double pu7 = ui->doubleSpinBox_pu7->value();
    double pu8 = ui->doubleSpinBox_pu8->value();
    double pu9 = ui->doubleSpinBox_pu9->value();
    double pu10 = ui->doubleSpinBox_pu10->value();
    double pu11 = ui->doubleSpinBox_pu11->value();
    double pu12 = ui->doubleSpinBox_pu12->value();
    double pu13 = ui->doubleSpinBox_pu13->value();
    double pu14 = ui->doubleSpinBox_pu14->value();
    double result = pu1+pu2+pu3+pu4+pu5+pu6+pu7+pu8+pu9+pu10+pu11+pu12+pu13+pu14;
    qDebug() << result << " :proformaRentsTotalOp";
    ui->doubleSpinBox_ptotalOp->setValue(result);
}

void MainWindow::updaterentalIncomeOp()
{
    QString rentalIncomeType = ui->comboBox_rentalIncome->currentText();
    double cRtotal = ui->doubleSpinBox_ctotalOp->value();
    double pRtotal = ui->doubleSpinBox_ptotalOp->value();
    double result = (rentalIncomeType == "Current")
                        ? (cRtotal) : (pRtotal);
    qDebug() << result << " :rentalIncomeOp";
    ui->doubleSpinBox_rentalIncomeOp->setValue(result);
}

void MainWindow::updatevacancyRateOp()
{
    if (!isVacancyRatePercentSet)
        return;
    double rentalIncome = ui->doubleSpinBox_rentalIncomeOp->value();
    double vacancyRatePercent = ui->doubleSpinBox_vacancyRateInper->value() / 100;
    double result = rentalIncome * vacancyRatePercent;
    qDebug() << result << " :vacancyRateOp";
    ui->doubleSpinBox_vacancyRateOp->setValue(result);
}


void MainWindow::updatetotalAnnualExpensesOp()
{
    if (!(isElectricGasSet && isWaterSewerSet && isGarbageSet && isHoaDuesSet &&
          isPMISet && isPropertyTaxesSet && isInsuranceSet &&
          isCapexSet && isMaintenanceSet && isPropertyManagementSet)) {
        return; // Don't proceed if any field isn't set yet
    }
    double electricGas = ui->doubleSpinBox_electricGas->value();
    double waterSewer = ui->doubleSpinBox_waterSewer->value();
    double garbage = ui->doubleSpinBox_garbage->value();
    double hoaDues = ui->doubleSpinBox_hoaDues->value();
    double pMIUnder20 = ui->doubleSpinBox_pmi->value();
    double annualVariableEx = (electricGas + waterSewer + garbage + hoaDues + pMIUnder20) * 12;
    double propertyTaxes = ui->doubleSpinBox_propertyTaxes->value();
    double insurance = ui->doubleSpinBox_insurance->value();
    double aCapexBud = ui->doubleSpinBox_annualCapexBudgetOp->value();
    double aMainBud = ui->doubleSpinBox_maintananceBudgetOp->value();
    double propertyManage = ui->doubleSpinBox_propertyManagementOp->value();
    double result = annualVariableEx + propertyTaxes + insurance + aCapexBud + aMainBud + propertyManage;
    qDebug() << result << " :totalAnnualExpensesOp";
    ui->doubleSpinBox_totalAnnualExpensesOp->setValue(result);
}
void MainWindow::updateGrossOperatingMonthlyIncomeOp()
{
    if (!isOtherIncomeSet)
        return;
    double rentalIncome = ui->doubleSpinBox_rentalIncomeOp->value();
    double otherIncome = ui->doubleSpinBox_otherIncome->value();
    double vacancyRate = ui->doubleSpinBox_vacancyRateOp->value();
    double result = rentalIncome + otherIncome - vacancyRate;
    qDebug() << result << " :GrossOperatingMonthlyIncomeOp";
    ui->doubleSpinBox_gOMIOp->setValue(result);
}

void MainWindow::updateannualCapexBudgetOp()
{
    if (!isAnnualCapexPercentSet)
        return;
    double annualCapexPercent = ui->doubleSpinBox_annualCapexbudgetInper->value() / 100;
    double rentalIncome = ui->doubleSpinBox_rentalIncomeOp->value();
    double result = annualCapexPercent * rentalIncome * 12;
    qDebug() << result << " :annualCapexBudgetOp";
    ui->doubleSpinBox_annualCapexBudgetOp->setValue(result);
}

void MainWindow::updatemaintananceBudgetOp()
{
    if (!isMaintananceBudgetPercentSet)
        return;
    double maintananceBudgetPercent = ui->doubleSpinBox_maintananceBudgetInper->value() / 100;
    double rentalIncome = ui->doubleSpinBox_rentalIncomeOp->value();
    double result = maintananceBudgetPercent * rentalIncome * 12;
    qDebug() << result << " :maintananceBudgetOp";
    ui->doubleSpinBox_maintananceBudgetOp->setValue(result);
}

void MainWindow::updateGrossAnnualOperatingIncomeOp()
{
    if (!isGrossOperatingMonthlyIncomeSet)
        return;
    double grossOperatingMonthlyIncome = ui->doubleSpinBox_gOMIOp->value();
    double result = grossOperatingMonthlyIncome * 12;
    qDebug() << result << " :GrossAnnualOperatingIncomeOp";
    ui->doubleSpinBox_gAOIOp->setValue(result);
}

void MainWindow::updatepropertyManagementOp()
{
    if (!isPropertyManagementPercentSet)
        return;
    double propertyManagementPercent = ui->doubleSpinBox_propertyManagementInper->value() / 100;
    double grossAnnualOI = ui->doubleSpinBox_gAOIOp->value();
    double result = propertyManagementPercent * grossAnnualOI;
    qDebug() << result << " :propertyManagementOp";
    ui->doubleSpinBox_propertyManagementOp->setValue(result);
}

void MainWindow::updateGrossAnnualOperatingExpensesOp()
{
    if (!isTotalAnnualExpensesOpSet)
        return;
    double totalAnnualExpenses = ui->doubleSpinBox_totalAnnualExpensesOp->value();
    double result = totalAnnualExpenses;
    qDebug() << result << " :GrossAnnualOperatingExpensesOp";
    ui->doubleSpinBox_gAOEOp->setValue(result);
}

void MainWindow::updatenetOperatingIncomeOp()
{
    if (!isGrossAnnualOperatingIncomeSet || !isGrossAnnualOperatingExpensesSet)
        return;
    double grossAnnualOperatingIncome = ui->doubleSpinBox_gAOIOp->value();
    double grossAnnualOperatingExpenses = ui->doubleSpinBox_gAOEOp->value();
    double result = grossAnnualOperatingIncome - grossAnnualOperatingExpenses;
    qDebug() << result << " :netOperatingIncomeOp";
    ui->doubleSpinBox_nOIOp->setValue(result);
}

void MainWindow::updateloanAmountMCOp()
{
    double loanAmountInDA = ui->doubleSpinBox_loanAmountOp->value();
    double result = loanAmountInDA;
    qDebug() << result << " :loanAmountMCOp";
    ui->doubleSpinBox_loanAmountMCOp->setValue(result);
}

void MainWindow::updateannualInterestRateMCOp()
{
    if (!isInterestRatePercentSet)
        return;
    double interestRatePercentInDA = ui->doubleSpinBox_interestRateInper->value();
    double result = interestRatePercentInDA;
    qDebug() << result << " :annualInterestRateMCOp";
    ui->doubleSpinBox_annualInterestRateMCOp->setValue(result);
}

void MainWindow::updatelifeloanOp()
{
    if (!isAmortizationSet)
        return;
    int amortization = ui->spinBox_amortization->value();
    int result = amortization;
    qDebug() << result << " :lifeloanOp";
    ui->spinBox_lifeloanOp->setValue(result);
}

void MainWindow::updatetotalnoofpaymentsOp()
{
    if (!isNumberOfPaymentsPerYearSet)
        return;
    int lifeLoan = ui->spinBox_lifeloanOp->value();
    int numberOfPaymentsPerYear = ui->spinBox_noofpaymentsperyear->value();
    int result = lifeLoan * numberOfPaymentsPerYear;
    qDebug() << result << " :totalnoofpaymentsOp";
    ui->spinBox_totalnoofpaymentsOp->setValue(result);
}

void MainWindow::updatepaymentperperiodOp()
{
    double loanAmount = ui->doubleSpinBox_loanAmountMCOp->value();
    double annualInterestRate = ui->doubleSpinBox_annualInterestRateMCOp->value() / 100;
    int numberOfPaymentsPerYear = ui->spinBox_noofpaymentsperyear->value();
    double noofPaymentsPerYear = numberOfPaymentsPerYear;
    int totalNumberOfPayments = ui->spinBox_totalnoofpaymentsOp->value();
    double ratePerPeriod = annualInterestRate / noofPaymentsPerYear;
    double result;
    // Prevent division by zero or invalid input
    if (loanAmount == 0 || totalNumberOfPayments == 0)
    {
        return;
    }
    else if (ratePerPeriod == 0)
    {
        result =  loanAmount / totalNumberOfPayments;
    }
    // PMT Formula in C++
    qDebug()<<ratePerPeriod<<"rateperperiod:";
    double deno = 1 - pow(1 + ratePerPeriod, -totalNumberOfPayments);


    result = (ratePerPeriod * loanAmount) / deno;

    precisePaymentPerPeriod = result;

    qDebug() << result << " :paymentPerPeriodOp";

    ui->doubleSpinBox_paymentperperiodOp->setValue(result);
}

void MainWindow::updateannualLoanPaymentsOp()
{
    if (!isPaymentPerPeriodSet)
        return;
    double paymentPerPeriod = precisePaymentPerPeriod;


    // Annual payment = monthly payment * 12 (absolute value)
    double result = std::abs(paymentPerPeriod * 12);

    qDebug() << result << " :annualLoanPaymentsOp";

    ui->doubleSpinBox_aLPOp->setValue(result);
}

void MainWindow::updatedownPaymentSFOp()
{
    if (!isDownPaymentPercentSFSet || !isPropertyPriceSet)
        return;
    double propertyPrice = ui->doubleSpinBox_propertyPrice->value();
    double downPaymentPercent = ui->doubleSpinBox_downPaymentInper->value() / 100;
    double result  = propertyPrice * downPaymentPercent;
    qDebug() << result << " :downPaymentSFOp";
    ui->doubleSpinBox_downPaymentSFOp->setValue(result);
}

void MainWindow::updateloanAmountSFOp()
{
    if (!isPropertyPriceSet || !isDownPaymentSFSet)
        return;
    double propertyPrice = ui->doubleSpinBox_propertyPrice->value();
    double downPayment = ui->doubleSpinBox_downPaymentSFOp->value();
    double result = propertyPrice - downPayment;
    qDebug() << result << " :loanAmountSFOp";
    ui->doubleSpinBox_loanAmountSFOp->setValue(result);
}

void MainWindow::updatemonthlyPaymentSFOp()
{
    if (!isInterestRateInPerSFSet || !isLoanTermInMonthsSet)
        return;

    double loanAmountInSF = ui->doubleSpinBox_loanAmountSFOp->value();
    double interestRateInPerSF = ui->doubleSpinBox_interestRateInper_2->value() / 100.0;
    int loanTermInMonths = ui->spinBox_loanTerm->value();

    if (loanAmountInSF <= 0 || loanTermInMonths <= 0)
        return;

    double payment;

    if (interestRateInPerSF == 0)
    {
        payment = loanAmountInSF / loanTermInMonths;
    }
    else
    {
        // Use long double for higher precision intermediate calculations
        long double loanAmount = static_cast<long double>(loanAmountInSF);
        long double annualRate = static_cast<long double>(interestRateInPerSF);
        long double termMonths = static_cast<long double>(loanTermInMonths);

        // Calculate monthly rate with higher precision
        long double monthlyRate = annualRate / 12.0L;

        // PMT formula with higher precision
        long double onePlusR = 1.0L + monthlyRate;
        long double powerTerm = std::pow(onePlusR, termMonths);
        qDebug() << QString("Powerterm with 15 decimals: %1").arg(powerTerm, 0, 'f', 6);
        long double denominator = powerTerm - 1.0L;
        long double numerator = loanAmount * monthlyRate * (powerTerm);

        long double precisePayment = numerator / denominator;

        payment = static_cast<double>(precisePayment);

        preciseMonthlyPaymentSF = payment;
    }

    // Debug with maximum precision
    qDebug() << QString("Payment with 15 decimals: %1").arg(payment, 0, 'f', 6);
    qDebug() << QString("Payment scientific: %1").arg(payment, 0, 'e', 6);
    ui->doubleSpinBox_monthlyPaymentSFOp->setValue(payment);
}


void MainWindow::updatesellerFinancePaymentsOp()
{
    if (!isMonthlyPaymentSFSet)
        return;
    double monthlyPaymentInSF = preciseMonthlyPaymentSF;


    double result = std::abs(monthlyPaymentInSF * 12);
    qDebug() << result << " :sellerFinancePaymentsOp";
    ui->doubleSpinBox_sFPOp->setValue(result);
}

void MainWindow::updatedebtServiceCoverageRatioOp()
{
    if (!isNetOperatingIncomeSet || !isAnnualLoanPaymentsSet)
        return;
    double netOperatingIncome = ui->doubleSpinBox_nOIOp->value();
    double annualLoanPayments = ui->doubleSpinBox_aLPOp->value();
    double result = netOperatingIncome / annualLoanPayments;
    qDebug() << result << " :debtServiceCoverageRatioOp";
    ui->doubleSpinBox_dSCROp->setValue(result);
}

void MainWindow::updatecapitalizationRateOp()
{
    if (!isContractPriceSet)
        return;
    double netOperatingIncome = ui->doubleSpinBox_nOIOp->value();
    double contractPrice = ui->doubleSpinBox_contractPrice->value();

    double result = (netOperatingIncome / contractPrice) * 100;
    qDebug() << result << " :capitalizationRateOp";
    ui->doubleSpinBox_capitalizationRateOp->setValue(result);
}

void MainWindow::updateannualCashFlowOp()
{
    if (!isGrossAnnualOISet || !isGrossAnnualOESet ||
        !isAnnualLoanPaymentsSet || !isSellerFinancePaymentsSet)
        return;
    double grossAnnualOI = ui->doubleSpinBox_gAOIOp->value();
    double grossAnnualOE = ui->doubleSpinBox_gAOEOp->value();
    double annualLoanPayments = ui->doubleSpinBox_aLPOp->value();
    double sellerFinancePayments = ui->doubleSpinBox_sFPOp->value();
    double result = grossAnnualOI - grossAnnualOE - annualLoanPayments - sellerFinancePayments;
    qDebug() << result << " :annualCashFlowOp";
    ui->doubleSpinBox_aCFOp->setValue(result);
}

void MainWindow::updatemonthlyCashFlowOp()
{
    if (!isAnnualCashFlowSet)
        return;
    double annualCashFlow =  ui->doubleSpinBox_aCFOp->value();
    double result = annualCashFlow / 12;
    qDebug() << result << " :monthlyCashFlowOp";
    ui->doubleSpinBox_mCFOp->setValue(result);
}

void MainWindow::updatenOI10CrARVOp()
{
    if (!isCapRatePercentSet)
        return;
    double grossAnnualOI = ui->doubleSpinBox_gAOIOp->value();
    double capRatePercent = ui->doubleSpinBox_capRateInper->value() / 100;

    double result = grossAnnualOI / capRatePercent;
    qDebug() << result << " :nOI10CrARVOp";
    ui->doubleSpinBox_nOI10CrARVOp->setValue(result);
}

void MainWindow::updatenOI7CrARVOp()
{
    if (!isCapRateAssetTypePercentSet)
        return;
    double grossAnnualOI = ui->doubleSpinBox_gAOIOp->value();
    double capRateAssetTypePercent = ui->doubleSpinBox_capRateCityAssetTypeInper->value() / 100;

    double result = grossAnnualOI / capRateAssetTypePercent;
    qDebug() << result << " :nOI7CrARVOp";
    ui->doubleSpinBox_nOI7CrARVOp->setValue(result);
}

void MainWindow::updatenOIafterTax10CrARVOp()
{
    if (!isCapRatePercentSet)
        return;
    double netOperatingIncome = ui->doubleSpinBox_nOIOp->value();
    double capRatePercent = ui->doubleSpinBox_capRateInper->value() / 100;

    double result = netOperatingIncome / capRatePercent;
    qDebug() << result << " :nOIafterTax10CrARVOp";
    ui->doubleSpinBox_nOIafterTax10CrARVOp->setValue(result);
}

void MainWindow::updatenOIafterTax7CrARVOp()
{
    if (!isCapRateAssetTypePercentSet)
        return;
    double netOperatingIncome = ui->doubleSpinBox_nOIOp->value();
    double capRateAssetTypePercent = ui->doubleSpinBox_capRateCityAssetTypeInper->value() / 100;

    double result = netOperatingIncome / capRateAssetTypePercent;
    qDebug() << result << " :nOIafterTax7CrARVOp";
    ui->doubleSpinBox_nOIafterTax7CrARVOp->setValue(result);
}

void MainWindow::updatenOI10CrMAOOp()
{
    if (!isRenovationsSet || !isNOI10ARVSet)
        return;
    double nOI10ARV = ui->doubleSpinBox_nOI10CrARVOp->value();
    double renovations = ui->doubleSpinBox_renovations->value();
    double result = (0.7 * nOI10ARV) - renovations;
    qDebug() << result << " :nOI10CrMAOOp";
    ui->doubleSpinBox_nOI10CrMAOOp->setValue(result);
}

void MainWindow::updatenOI7CrMAOOp()
{
    if (!isRenovationsSet || !isNOI7ARVSet)
        return;
    double nOI7ARV = ui->doubleSpinBox_nOI7CrARVOp->value();
    double renovations = ui->doubleSpinBox_renovations->value();
    double result = (0.7 * nOI7ARV) - renovations;
    qDebug() << result << " :nOI7CrMAOOp";
    ui->doubleSpinBox_nOI7CrMAOOp->setValue(result);
}

void MainWindow::updatenOIafterTax10CrMAOOp()
{
    if (!isRenovationsSet || !isNOIAfterTax10CrARVSet)
        return;
    double nOI10AfterTaxARV = ui->doubleSpinBox_nOIafterTax10CrARVOp->value();
    double renovations = ui->doubleSpinBox_renovations->value();
    double result = (0.7 * nOI10AfterTaxARV) - renovations;
    qDebug() << result << " :nOIafterTax10CrMAOOp";
    ui->doubleSpinBox_nOIafterTax10CrMAOOp->setValue(result);
}

void MainWindow::updatenOIafterTax7CrMAOOp()
{
    if (!isRenovationsSet || !isNOIAfterTax7CrARVSet)
        return;
    double nOI7AfterTaxARV = ui->doubleSpinBox_nOIafterTax7CrARVOp->value();
    double renovations = ui->doubleSpinBox_renovations->value();
    double result = (0.7 * nOI7AfterTaxARV) - renovations;
    qDebug() << result << " :nOIafterTax7CrMAOOp";
    ui->doubleSpinBox_nOIafterTax7CrMAOOp->setValue(result);
}

void MainWindow::updateballonAmountSFOp()
{
    if (!isBallonDueInMonthsSet)
        return;
    double loanAmountInSF = ui->doubleSpinBox_loanAmountSFOp->value();
    double interestRateInSF = ui->doubleSpinBox_interestRateInper_2->value() / 100.0;
    double monthlyPaymentInSF = preciseMonthlyPaymentSF;
    QString ballonPaymentType = ui->comboBox_ballonPayment->currentText();
    int ballonDueInMonths = ui->spinBox_ballonDue->value();
    double result = 0.0;

    if (ballonPaymentType == "Yes")
    {
        double monthlyRate = interestRateInSF / 12.0;
        int n = ballonDueInMonths;
        double pmt = monthlyPaymentInSF;
        double pv = loanAmountInSF;


        // FV formula: FV = -(PV * (1 + r)^n + PMT * [((1 + r)^n - 1) / r])
        result = pmt * ((pow(1 + monthlyRate, n) - 1) / monthlyRate) + (-pv) * pow(1 + monthlyRate, n);
        result = qAbs(result);
    }

    qDebug() << result << " :ballonAmountSFOp";
    ui->doubleSpinBox_ballonAmountSFOp->setValue(result);
}

void MainWindow::updatesumofpaymentsOp()
{
    if (!isPaymentPerPeriodSet || !isTotalNumberOfPaymentsSet)
        return;
    double paymentPerPeriod = precisePaymentPerPeriod;
    qDebug()<<paymentPerPeriod<<"paymentper:";
    int totalNumberOfPayments = ui->spinBox_totalnoofpaymentsOp->value();
    double result = paymentPerPeriod * totalNumberOfPayments;
    qDebug() << result << " :sumofpaymentsOp";
    ui->doubleSpinBox_sumofpaymentsOp->setValue(result);
}

void MainWindow::updateinterestcostOp()
{
    if (!isSumOfPaymentsSet || !isLoanAmountMCSet)
        return;
    double sumOfPayments = ui->doubleSpinBox_sumofpaymentsOp->value();
    double loanAmountInMC = ui->doubleSpinBox_loanAmountMCOp->value();
    double result = sumOfPayments - loanAmountInMC;
    qDebug() << result << " :interestcostOp";
    ui->doubleSpinBox_interestcostOp->setValue(result);
}

void MainWindow::updatePaymentSchedule()
{
    double annualInterestRateMC = ui->doubleSpinBox_annualInterestRateMCOp->value() / 100; // B5
    int numberOfPaymentsPerYear = ui->spinBox_noofpaymentsperyear->value();          // B7
    double paymentPerPeriod = precisePaymentPerPeriod;       // B9
    QDate selectedDate = ui->dateEdit_myDate->date();                                // E7
    int totalNumberOfPayments = ui->spinBox_totalnoofpaymentsOp->value();            // B8
    double loanAmountMC = ui->doubleSpinBox_loanAmountMCOp->value();                 // K7

    // Prepare table headers
    ui->tableWidget_schedule->clear();
    ui->tableWidget_schedule->setRowCount(totalNumberOfPayments);
    ui->tableWidget_schedule->setColumnCount(7);
    QStringList headers = {"Date", "Payment No.", "Payment Amount", "Interest", "Principal", "Loan Balance", "Condition"};
    ui->tableWidget_schedule->setHorizontalHeaderLabels(headers);

    ui->tableWidget_schedule->setColumnHidden(6, true);

    QDate currentDate = selectedDate;
    double remainingLoan = loanAmountMC;
    currentDate = currentDate.addMonths(1);
    for (int i = 0; i < totalNumberOfPayments; ++i)
    {
        int paymentNo = i + 1;

        // Interest calculation
        double interest = remainingLoan * annualInterestRateMC / numberOfPaymentsPerYear;
        qDebug()<<interest<<"interestbefore:";
        interest = std::ceil(interest * 10000.0) / 10000.0;
        qDebug()<<interest<<"interestafter:";

        // Payment Amount
        double expectedPayment = remainingLoan + interest;
        double paymentAmount = (paymentPerPeriod < expectedPayment) ? paymentPerPeriod : expectedPayment;
        qDebug()<<paymentAmount<<"paymentAmountBefore:";
        paymentAmount = std::ceil(paymentAmount * 1000.0) / 1000.0;
        qDebug()<<paymentAmount<<"paymentAmountAfter:";
        // Principal
        double principal = paymentAmount - interest;
        qDebug()<<principal<<"principal:";
        // New remaining loan
        remainingLoan = qAbs(remainingLoan - principal);

        // Round loan for condition check
        bool condition = qRound(remainingLoan * 10) / 10.0 == 0;

        // Insert and center-align table cells
        auto insertItem = [&](int row, int col, const QString &text) {
            QTableWidgetItem *item = new QTableWidgetItem(text);
            item->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget_schedule->setItem(row, col, item);
        };

        insertItem(i, 0, currentDate.toString("dd-MM-yyyy"));
        insertItem(i, 1, QString::number(paymentNo));
        insertItem(i, 2, QString::number(paymentAmount, 'f', 2));
        insertItem(i, 3, QString::number(interest, 'f', 2));
        insertItem(i, 4, QString::number(principal, 'f', 2));
        insertItem(i, 5, QString::number(remainingLoan, 'f', 2));
        insertItem(i, 6, condition ? "TRUE" : "FALSE");

        // Move to next month
        currentDate = currentDate.addMonths(1);
    }
    loanEndDate = currentDate.addMonths(-1);
}

void MainWindow::updateprincipleReduction1Op()
{
    double sum = 0.0;

    // Get the number of rows in the table (in case there are fewer than 12)
    int rowCount = ui->tableWidget_schedule->rowCount();
    int rowsToRead = qMin(12, rowCount);  // Read up to 12 rows only

    for (int i = 0; i < rowsToRead; ++i)
    {
        QTableWidgetItem *item = ui->tableWidget_schedule->item(i, 4); // Principal column (index 4)

        if (item)
        {
            bool ok;
            double value = item->text().toDouble(&ok);
            if (ok)
                sum += value;
        }
    }

    qDebug() << sum << " :principleReduction1Op";

    // Set the result in appropriate output field
    ui->doubleSpinBox_principleReduction1Op->setValue(sum);
}


void MainWindow::updateappreciation1Op()
{
    if (!isContractPriceSet || !isAnnualAppreciationRateSet)
        return;
    double contractPrice = ui->doubleSpinBox_contractPrice->value();
    double annualAppreciationRatePercent = ui->doubleSpinBox_annualARInper->value() / 100;
    double result = contractPrice * annualAppreciationRatePercent;
    qDebug() << result << " :appreciation1Op";
    ui->doubleSpinBox_appreciation1Op->setValue(result);
}

void MainWindow::updatecashoncashReturnOp()
{
    if (!isAnnualCashFlowSet || !isTotalInitialInvestmentSet)
        return;
    double annualCashFlow = ui->doubleSpinBox_aCFOp->value();
    double totalInitialInvestment = ui->doubleSpinBox_totalInitialInvestmentOp->value();
    double result = annualCashFlow / totalInitialInvestment;
    result = result * 100;
    qDebug() << result << " :cashoncashReturnOp";
    ui->doubleSpinBox_cashoncashReturnOp->setValue(result);
}

void MainWindow::updateprincipleReductionOp()
{
    if (!isAnnualCashFlowSet || !isPrincipleReduction1Set || !isTotalInitialInvestmentSet)
        return;
    double annualCashFlow = ui->doubleSpinBox_aCFOp->value();
    double principleReductionIn1Year = ui->doubleSpinBox_principleReduction1Op->value();
    double totalInitialInvestment = ui->doubleSpinBox_totalInitialInvestmentOp->value();
    double result = (annualCashFlow + principleReductionIn1Year) / totalInitialInvestment;
    result = result * 100;
    qDebug() << result << " :principleReductionOp";
    ui->doubleSpinBox_principleReductionOp->setValue(result);
}

void MainWindow::updateappreciation()
{
    if (!isAppreciation1Set || !isAnnualCashFlowSet || !isTotalInitialInvestmentSet)
        return;
    double appreciationIn1Year = ui->doubleSpinBox_appreciation1Op->value();
    double annualCashFlow = ui->doubleSpinBox_aCFOp->value();
    double totalInitialInvestment = ui->doubleSpinBox_totalInitialInvestmentOp->value();
    double result = (appreciationIn1Year + annualCashFlow) / totalInitialInvestment;
    result = result * 100;
    qDebug() << result << " :appreciation";
    ui->doubleSpinBox_appreciation->setValue(result);
}

void MainWindow::updatetotalReturnOnInvestment()
{
    if (!isAnnualCashFlowSet || !isPrincipleReduction1Set ||
        !isAppreciation1Set || !isTotalInitialInvestmentSet)
        return;
    double annualCashFlow = ui->doubleSpinBox_aCFOp->value();
    double principleReductionIn1Year = ui->doubleSpinBox_principleReduction1Op->value();
    double appreciationIn1Year = ui->doubleSpinBox_appreciation1Op->value();
    double totalInitialInvestment = ui->doubleSpinBox_totalInitialInvestmentOp->value();
    double result = (annualCashFlow + principleReductionIn1Year + appreciationIn1Year) / totalInitialInvestment;
    result = result * 100;
    qDebug() << result << " :totalReturnOnInvestment";
    ui->doubleSpinBox_totalROIOp->setValue(result);
}
MainWindow::~MainWindow()
{
    delete ui;
}

