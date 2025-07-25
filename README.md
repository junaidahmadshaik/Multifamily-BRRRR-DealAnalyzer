# Multifamily BRRRR Deal Analyzer

This is a cross-platform financial analysis tool designed to help real estate investors evaluate multifamily properties using the **BRRRR strategy (Buy, Rehab, Rent, Refinance, Repeat)**. Built with **Qt (C++)** and deployed to **WebAssembly**, this application offers a responsive and interactive graphical user interface (GUI) for detailed financial modeling, accessible on both desktop and web platforms.

## Table of Contents

- [Features](#features)
- [Technologies Used](#technologies-used)
- [Application Tabs](#application-tabs)
  - [Deal Analyzer](#deal-analyzer)
  - [Seller Financing](#seller-financing)
  - [Mortgage Calculator](#mortgage-calculator)
  - [Revisions](#revisions)
- [Key Metrics Calculated](#key-metrics-calculated)
- [Getting Started](#getting-started)
- [How to Use](#how-to-use)
- [Contributing](#contributing)

---

## Features

* **Cross-Platform Compatibility:** Available as a desktop application and a web application (via WebAssembly).
* **Interactive GUI:** Responsive user interface with dynamic input validation for real-time feedback.
* **Live Calculation:** Instantly calculates key real estate metrics as inputs are provided.
* **Automated Schedules:** Generates detailed loan amortization schedules and interest breakdowns.
* **Revisions Module:** Tracks financial snapshots with timestamps and formatted summaries, allowing users to save, review, and delete entries.
* **Session Persistence:** Saved revisions persist across sessions, page refreshes, and application restarts using `localStorage`.
* **CSV Export:** Export revision history to a spreadsheet-compatible CSV format for reporting and offline analysis.

---

## Technologies Used

* **Qt 6.7.3 (C++)**: The primary framework for developing the cross-platform application and its GUI.
* **WebAssembly**: Enables web deployment of the C++ application, providing near-native performance in web browsers.
* **QtWidgets**: Used for building the desktop-style graphical user interface elements.
* **Emscripten**: The LLVM-based compiler toolchain used to compile C++ code to WebAssembly.
* **HTML5 localStorage**: Integrated for client-side data persistence, ensuring saved revisions are retained across sessions.
* **Key Qt & Emscripten Headers**:
    * `QFileDialog`: For file system interactions (e.g., CSV export).
    * `QTextDocument`: For rich text handling.
    * `QJsonDocument`, `QJsonObject`, `QJsonArray`, `QJsonValue`: For structured data serialization and deserialization (e.g., saving revision data).
    * `emscripten.h`, `emscripten/bind.h`: For integrating C++ code with JavaScript and accessing browser APIs like `localStorage`.

---

## Application Tabs

The application is organized into four main tabs to streamline the analysis process:

### Deal Analyzer

This is the core of the BRRRR analysis, where you input various financial parameters related to a property and calculate its potential profitability.

**Key Inputs & Outputs:**

* **Property Information:** Property Address, Property Notes, Asking Price, Contract Price.
* **Initial Investment:** Renovation Type, Renovation Estimate, Lease Up Fees, Down Payment (%), Down Payment ($), Closing costs (%), Closing costs ($), **Total Initial Investment**.
* **Income Analysis:** Rental Income Type, Rental Income, Other Income, Vacancy Rate (%), Vacancy Rate ($), **Gross Operating Monthly Income**, **Gross Annual Operating Income**.
* **Expense Analysis:** Electric + Gas, Water + Sewer, Garbage, HOA Dues, PMI, Property Taxes, Insurance, Annual CapEx Budget (%), Maintenance Budget (%), Property Management (%), **Total Annual Expenses**, **Gross Annual Operating Expenses**.
* **Loan Details:** Loan Amount, Loan Points (%), Amortization (yrs), Interest Rate (%), Annual Appreciation Rate (%).
* **Performance Metrics:**
    * **Net Operating Income (NOI)**
    * **Cap Rate (%)**
    * Cap Rate (City/Asset Type) (%)
    * NOI 10% Cap Rate ARV, NOI 7% Cap Rate ARV
    * NOI (after taxes & insurance) / 10% Cap Rate ARV, NOI (after taxes & insurance) / 7% Cap Rate ARV
    * NOI 10% Cap Rate MAO, NOI 7% Cap Rate MAO
    * NOI (after taxes & insurance) / 10% Cap Rate MAO, NOI (after taxes & insurance) / 7% Cap Rate MAO
    * Annual Loan Payments, Seller Finance Payments
    * **DSCR (Debt Service Coverage Ratio)**
    * **Capitalization Rate**
    * **Annual Cash Flow**, **Monthly Cash Flow**
    * Principal Reduction In First Year, Appreciation In First Year
    * **Cash on Cash Return**
    * **Total Principal Reduction**, **Total Appreciation**
    * **Total Return On Investment**
    * Total Current Rents, Total Proforma Rents

### Seller Financing

This tab helps in understanding and calculating specifics related to seller-financed deals, particularly the balloon payment.

**Key Inputs & Outputs:**

* Property Price
* Down Payment (%), Down Payment ($)
* Loan Amount
* Interest Rate
* Loan Term (yrs)
* Monthly Payment
* Balloon Type (Yes/No)
* Balloon Due (Month)
* **Balloon Amount**

### Mortgage Calculator

A utility tab to generate detailed payment schedules for a given loan, showing the breakdown of principal and interest.

**Key Inputs & Outputs:**

* Loan Amount
* Annual Interest Rate
* Loan Life (yrs)
* Payments/Year
* **Total Payments/Year**
* **Payment Per Period**
* **Sum of Payments (Total Cost of Loan)**
* **Interest Cost**
* Loan Starting Date
* Loan Ending Date

### Revisions

This module provides a robust history tracking system for your financial analyses.

**Functionality:**

* **Save Snapshots:** Users can save the current state of their financial calculations with an associated timestamp.
* **Table-Based History:** Revisions are displayed in a clear, organized table format.
* **Formatted Summaries:** Each saved revision includes a formatted summary for quick review.
* **Delete Entries:** Specific revision entries can be deleted as needed.
* **CSV Export:** Export the entire revision history to a CSV file for external analysis or record-keeping.
* **Session Persistence:** All saved revisions are automatically stored and loaded across application sessions, page refreshes, and restarts thanks to `localStorage` integration.

---

## Key Metrics Calculated

The tool provides insights into various crucial real estate investment metrics, including:

* **DSCR (Debt Service Coverage Ratio):** A measure of the cash flow available to pay current debt obligations.
* **ROI (Return on Investment):** The overall profitability of an investment.
* **NOI (Net Operating Income):** A property's income after accounting for all operating expenses, but before debt service and income taxes.
* **Cash Flow:** The net amount of cash and cash equivalents being transferred into and out of the property.
* **Loan Amortization:** A detailed schedule of loan payments, showing the principal and interest breakdown over time.
* **Cap Rate (Capitalization Rate):** A valuation measure used to compare different investment properties.

---

## Getting Started

This guide explains how to **build and run the Multifamily BRRRR Deal Analyzer** for both **desktop** and **web (WASM)** environments using Qt 6.7.3 and Emscripten.

### For Desktop (Windows - MinGW 64-bit)

#### Prerequisites:
- [Qt 6.7.3](https://www.qt.io/download) with **MinGW 64-bit** kit installed.
- Qt Creator (recommended for ease of building and deploying).
- CMake (Auto-detected by Qt Creator or installed manually from [cmake.org](https://cmake.org/)).

#### Steps:

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/junaidahmadshaik/Multifamily-BRRRR-DealAnalyzer.git
   cd Multifamily-BRRRR-DealAnalyzer
   ```

2. **Open the Project in Qt Creator:**
   - Open `DealAnalyzer.pro` using Qt Creator.
   - Select the **Desktop MinGW 64-bit** kit.
   - Let Qt auto-detect paths and kits.

3. **Build and Run:**
   - Click `Build > Build Project`.
   - Then click `Run`. The app will launch with the GUI interface.

### For Web (WebAssembly via Emscripten)

#### Prerequisites:
- Qt 6.7.3 with **WebAssembly single-threaded** kit.
- **Emscripten SDK 3.1.50** (manually installed at `C:\Users\junai\Desktop\emsdk`).
- A simple HTTP server for local testing (e.g., Python’s `http.server`).

### Setup Emscripten SDK (Manual Installation)

If you already cloned the repo, follow these steps to setup `emsdk`:

1. **Open Command Prompt and Run:**
   ```bash
   cd C:\Users\junai\Desktop\emsdk
   emsdk activate 3.1.50
   emsdk_env.bat
   ```

2. **Test the installation:**
   ```bash
   emcc --version
   ```
   You should see output like:
   ```
   emcc (Emscripten gcc/clang-like replacement) 3.1.50
   ```

### Build for Web using Qt Creator

1. **Open `DealAnalyzer.pro` in Qt Creator.**

2. **Select the WebAssembly Kit:**
   - Choose `Qt 6.7.3 WebAssembly (Single-Threaded)`.

3. **Build the Project:**
   - Go to `Build > Build Project`.
   - The build output folder will include:
     ```
     DealAnalyzer.html
     DealAnalyzer.js
     DealAnalyzer.wasm
     qtloader.js
     ```

> Note: WebAssembly apps run best on modern browsers like **Chrome** or **Edge**. Firefox may need tuning for Qt WASM.

### Output Files Summary

| File Name             | Purpose                        |
|-----------------------|--------------------------------|
| `DealAnalyzer.html`   | Main launcher page             |
| `DealAnalyzer.js`     | JavaScript glue code           |
| `DealAnalyzer.wasm`   | WebAssembly binary (core app)  |
| `qtloader.js`         | Qt-specific loader script      |


### Optional: Optimize WebAssembly Build (Advanced)

To reduce load time and improve performance:

- **Enable size optimization** in `.pro` file:
  ```pro
  QMAKE_LFLAGS += -Os --closure 1 --llvm-lto 1
  ```

- **Compress `.wasm` file** using Brotli or Gzip on your deployment server:
  ```bash
  brotli DealAnalyzer.wasm -o DealAnalyzer.wasm.br
  # or
  gzip -k DealAnalyzer.wasm
  ```

> These steps are typically done during deployment (e.g., on Ubuntu server using Nginx with Brotli module).

---

## How to Use

1.  Navigate between the **"Deal Analyzer"**, **"Seller Financing"**, **"Mortgage Calculator"**, and **"Revisions"** tabs.
2.  **Input Data:** Enter relevant financial data into the input fields on each tab. The application will dynamically update calculations.
3.  **Save Revisions:** In the "Revisions" tab, click the "Save Current Deal" (or similar) button to store the current financial snapshot.
4.  **Manage Revisions:** Review, delete, or export saved revisions from the "Revisions" tab.
5.  **Export Data:** Use the CSV export feature in the "Revisions" tab to download your history.

---

## Contributing

Contributions are welcome! If you have suggestions for improvements, bug reports, or would like to contribute code, please feel free to open an issue or submit a pull request.

---

