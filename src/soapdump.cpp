#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <regex>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <getopt.h>
#include <cstring>

// Transaction data structure
struct Transaction {
    int transNum;
    std::string amount;
    std::string currency;
    std::string firstName;
    std::string lastName;
    std::string street;
    std::string city;
    std::string state;
    std::string zip;
    std::string ccType;
    std::string ccLast4;
    std::string expMonth;
    std::string expYear;
    std::string cvv;
    std::string transId;
    std::string status;
    std::string corrId;
    std::string procAmount;
};

// Response data structure
struct Response {
    std::string transId;
    std::string status;
    std::string corrId;
    std::string procAmount;
};

// Function prototypes
void showHelp(const char* programName);
void generateBashCompletion();
void generateZshCompletion();
void generateFishCompletion();
void generateManPage();
std::string extractXmlValue(const std::string& xml, const std::string& tag);
std::string extractXmlAttribute(const std::string& xml, const std::string& attribute);
std::vector<std::string> extractRequests(const std::string& logContent);
std::vector<std::string> extractResponses(const std::string& logContent);
std::vector<Response> parseResponses(const std::vector<std::string>& responseXmls);
std::vector<Transaction> parseTransactions(const std::vector<std::string>& requestXmls, const std::vector<Response>& responses);
void outputRawData(const std::vector<Transaction>& transactions);
void outputSummary(const std::vector<Transaction>& transactions);

int main(int argc, char* argv[]) {
    // Default options
    bool summaryOnly = false;
    std::string logFile;

    // Parse command line options
    static struct option longOptions[] = {
        {"help", no_argument, 0, 'h'},
        {"summary", no_argument, 0, 's'},
        {"raw", no_argument, 0, 'r'},
        {"generate-bash-completion", no_argument, 0, 0},
        {"generate-zsh-completion", no_argument, 0, 0},
        {"generate-fish-completion", no_argument, 0, 0},
        {"man", no_argument, 0, 0},
        {0, 0, 0, 0}
    };

    int optionIndex = 0;
    int opt;
    while ((opt = getopt_long(argc, argv, "hsr", longOptions, &optionIndex)) != -1) {
        switch (opt) {
            case 0:
                // Long options without short equivalents
                if (strcmp(longOptions[optionIndex].name, "generate-bash-completion") == 0) {
                    generateBashCompletion();
                    return 0;
                } else if (strcmp(longOptions[optionIndex].name, "generate-zsh-completion") == 0) {
                    generateZshCompletion();
                    return 0;
                } else if (strcmp(longOptions[optionIndex].name, "generate-fish-completion") == 0) {
                    generateFishCompletion();
                    return 0;
                } else if (strcmp(longOptions[optionIndex].name, "man") == 0) {
                    generateManPage();
                    return 0;
                }
                break;
            case 'h':
                showHelp(argv[0]);
                return 0;
            case 's':
                summaryOnly = true;
                break;
            case 'r':
                summaryOnly = false;
                break;
            case '?':
                std::cerr << "Unknown option: " << static_cast<char>(optopt) << std::endl;
                showHelp(argv[0]);
                return 1;
            default:
                break;
        }
    }

    // Get logfile name
    if (optind < argc) {
        logFile = argv[optind];
    } else {
        std::cerr << "Error: No logfile specified" << std::endl;
        showHelp(argv[0]);
        return 1;
    }

    // Check if file exists
    std::ifstream file(logFile);
    if (!file.is_open()) {
        std::cerr << "Error: File '" << logFile << "' not found" << std::endl;
        return 1;
    }

    // Read the entire file
    std::string logContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // Extract requests and responses
    std::vector<std::string> requestXmls = extractRequests(logContent);
    std::vector<std::string> responseXmls = extractResponses(logContent);

    // Parse responses
    std::vector<Response> responses = parseResponses(responseXmls);

    // Parse transactions
    std::vector<Transaction> transactions = parseTransactions(requestXmls, responses);

    // Output data
    if (summaryOnly) {
        outputSummary(transactions);
    } else {
        outputRawData(transactions);
    }

    return 0;
}

void showHelp(const char* programName) {
    std::cout << "PayPal SOAP Log Parser\n\n";
    std::cout << "USAGE:\n";
    std::cout << "    " << programName << " [OPTIONS] <logfile>\n\n";
    std::cout << "OPTIONS:\n";
    std::cout << "    -h, --help                    Show this help message\n";
    std::cout << "    -s, --summary                 Show summary statistics only\n";
    std::cout << "    -r, --raw                     Output raw structured data (default)\n";
    std::cout << "    --generate-bash-completion    Generate Bash completion script\n";
    std::cout << "    --generate-zsh-completion     Generate Zsh completion script\n";
    std::cout << "    --generate-fish-completion    Generate Fish completion script\n";
    std::cout << "    --man                         Generate man page\n\n";
    std::cout << "OUTPUT FORMAT:\n";
    std::cout << "    TRANS_NUM|AMOUNT|CURRENCY|FIRSTNAME|LASTNAME|STREET|CITY|STATE|ZIP|CCTYPE|CCLAST4|EXPMONTH|EXPYEAR|CVV|TRANSID|STATUS|CORRID|PROC_AMOUNT\n\n";
    std::cout << "FIELD DESCRIPTIONS:\n";
    std::cout << "    TRANS_NUM    - Transaction sequence number\n";
    std::cout << "    AMOUNT       - Order total amount\n";
    std::cout << "    CURRENCY     - Currency code (USD, etc)\n";
    std::cout << "    FIRSTNAME    - Customer first name\n";
    std::cout << "    LASTNAME     - Customer last name\n";
    std::cout << "    STREET       - Street address\n";
    std::cout << "    CITY         - City name\n";
    std::cout << "    STATE        - State/Province code\n";
    std::cout << "    ZIP          - Postal code\n";
    std::cout << "    CCTYPE       - Credit card type (Visa, MasterCard, etc)\n";
    std::cout << "    CCLAST4      - Last 4 digits of credit card\n";
    std::cout << "    EXPMONTH     - Card expiration month\n";
    std::cout << "    EXPYEAR      - Card expiration year\n";
    std::cout << "    CVV          - CVV code\n";
    std::cout << "    TRANSID      - PayPal transaction ID\n";
    std::cout << "    STATUS       - Transaction status (Success/Failure)\n";
    std::cout << "    CORRID       - Correlation ID\n";
    std::cout << "    PROC_AMOUNT  - Actually processed amount\n\n";
    std::cout << "EXAMPLES:\n";
    std::cout << "    # Get all transactions\n";
    std::cout << "    " << programName << " payments.log\n\n";
    std::cout << "    # Get only successful transactions\n";
    std::cout << "    " << programName << " payments.log | grep Success\n\n";
    std::cout << "    # Count transactions by state\n";
    std::cout << "    " << programName << " payments.log | cut -d'|' -f8 | sort | uniq -c | sort -nr\n\n";
    std::cout << "    # Find largest transaction\n";
    std::cout << "    " << programName << " payments.log | sort -t'|' -k2 -nr | head -1\n\n";
    std::cout << "    # Get transactions over $500\n";
    std::cout << "    " << programName << " payments.log | awk -F'|' '$2 > 500'\n\n";
    std::cout << "    # Summary stats\n";
    std::cout << "    " << programName << " -s payments.log\n";
}

void generateBashCompletion() {
    std::cout << R"(
_soapdump_completions()
{
    local cur prev opts
    COMPREPLY=()
    cur="${COMP_WORDS[COMP_CWORD]}"
    prev="${COMP_WORDS[COMP_CWORD-1]}"
    opts="--help --summary --raw --generate-bash-completion --generate-zsh-completion --generate-fish-completion --man"

    if [[ ${cur} == -* ]] ; then
        COMPREPLY=( $(compgen -W "${opts}" -- ${cur}) )
        return 0
    fi

    # Complete with log files if not an option
    if [[ ${cur} != -* ]]; then
        COMPREPLY=( $(compgen -f -X '!*.log' -- ${cur}) )
        return 0
    fi
}

complete -F _soapdump_completions soapdump
)" << std::endl;
}

void generateZshCompletion() {
    std::cout << R"(
#compdef soapdump

_arguments -s -S \
  '(-h --help)'{-h,--help}'[Show help message]' \
  '(-s --summary)'{-s,--summary}'[Show summary statistics only]' \
  '(-r --raw)'{-r,--raw}'[Output raw structured data (default)]' \
  '--generate-bash-completion[Generate Bash completion script]' \
  '--generate-zsh-completion[Generate Zsh completion script]' \
  '--generate-fish-completion[Generate Fish completion script]' \
  '--man[Generate man page]' \
  '*:log file:_files -g "*.log"'
)" << std::endl;
}

void generateFishCompletion() {
    std::cout << R"(
function __fish_soapdump_no_subcommand
    set cmd (commandline -opc)
    if [ (count $cmd) -eq 1 ]
        return 0
    end
    return 1
end

complete -c soapdump -s h -l help -d "Show help message"
complete -c soapdump -s s -l summary -d "Show summary statistics only"
complete -c soapdump -s r -l raw -d "Output raw structured data"
complete -c soapdump -l generate-bash-completion -d "Generate Bash completion script"
complete -c soapdump -l generate-zsh-completion -d "Generate Zsh completion script"
complete -c soapdump -l generate-fish-completion -d "Generate Fish completion script"
complete -c soapdump -l man -d "Generate man page"
complete -c soapdump -n "__fish_soapdump_no_subcommand" -a "*.log" -d "Log file"
)" << std::endl;
}

void generateManPage() {
    std::cout << R"(.TH SOAPDUMP 1 "September 2025" "soapdump 0.1.0" "User Commands"
.SH NAME
soapdump \- PayPal SOAP log parser
.SH SYNOPSIS
.B soapdump
[\fIOPTIONS\fR] \fILOGFILE\fR
.SH DESCRIPTION
.B soapdump
is a high-performance PayPal SOAP log parser that extracts transaction data from log files and outputs it in a structured format.
.SH OPTIONS
.TP
.BR \-h ", " \-\-help
Show help message and exit.
.TP
.BR \-s ", " \-\-summary
Show summary statistics only.
.TP
.BR \-r ", " \-\-raw
Output raw structured data (default).
.TP
.BR \-\-generate-bash-completion
Generate Bash completion script.
.TP
.BR \-\-generate-zsh-completion
Generate Zsh completion script.
.TP
.BR \-\-generate-fish-completion
Generate Fish completion script.
.TP
.BR \-\-man
Generate this man page.
.SH OUTPUT FORMAT
The output is pipe-separated with the following fields:
.PP
TRANS_NUM|AMOUNT|CURRENCY|FIRSTNAME|LASTNAME|STREET|CITY|STATE|ZIP|CCTYPE|CCLAST4|EXPMONTH|EXPYEAR|CVV|TRANSID|STATUS|CORRID|PROC_AMOUNT
.SH EXAMPLES
.TP
Get all transactions:
.B soapdump payments.log
.TP
Get only successful transactions:
.B soapdump payments.log | grep Success
.TP
Count transactions by state:
.B soapdump payments.log | cut -d'|' -f8 | sort | uniq -c | sort -nr
.TP
Find largest transaction:
.B soapdump payments.log | sort -t'|' -k2 -nr | head -1
.TP
Get transactions over $500:
.B soapdump payments.log | awk -F'|' '$2 > 500'
.TP
Summary stats:
.B soapdump -s payments.log
.SH AUTHOR
Kieran Klukas <me@dunkirk.sh>
.SH COPYRIGHT
Copyright \(co 2025 Kieran Klukas. License: MIT.
)" << std::endl;
}

std::string extractXmlValue(const std::string& xml, const std::string& tag) {
    std::regex pattern("<" + tag + "(?:[^>]*)>([^<]*)</" + tag + ">");
    std::smatch match;
    if (std::regex_search(xml, match, pattern) && match.size() > 1) {
        return match[1].str();
    }
    return "";
}

std::string extractXmlAttribute(const std::string& xml, const std::string& attribute) {
    std::regex pattern(attribute + "=\"([^\"]*)\"");
    std::smatch match;
    if (std::regex_search(xml, match, pattern) && match.size() > 1) {
        return match[1].str();
    }
    return "";
}

std::vector<std::string> extractRequests(const std::string& logContent) {
    std::vector<std::string> requests;
    std::regex pattern("PPAPIService: Request: (.*)");

    std::string::const_iterator searchStart(logContent.cbegin());
    std::smatch match;
    while (std::regex_search(searchStart, logContent.cend(), match, pattern)) {
        if (match.size() > 1) {
            requests.push_back(match[1].str());
        }
        searchStart = match.suffix().first;
    }

    return requests;
}

std::vector<std::string> extractResponses(const std::string& logContent) {
    std::vector<std::string> responses;
    std::regex pattern("PPAPIService: Response: <\\?.*\\?>(.*)");

    std::string::const_iterator searchStart(logContent.cbegin());
    std::smatch match;
    while (std::regex_search(searchStart, logContent.cend(), match, pattern)) {
        if (match.size() > 1) {
            responses.push_back(match[1].str());
        }
        searchStart = match.suffix().first;
    }

    return responses;
}

std::vector<Response> parseResponses(const std::vector<std::string>& responseXmls) {
    std::vector<Response> responses;

    for (const auto& xml : responseXmls) {
        Response response;
        response.transId = extractXmlValue(xml, "TransactionID");
        response.status = extractXmlValue(xml, "Ack");
        response.corrId = extractXmlValue(xml, "CorrelationID");
        response.procAmount = extractXmlValue(xml, "Amount");

        responses.push_back(response);
    }

    return responses;
}

std::vector<Transaction> parseTransactions(const std::vector<std::string>& requestXmls, const std::vector<Response>& responses) {
    std::vector<Transaction> transactions;
    int transNum = 1;

    for (size_t i = 0; i < requestXmls.size(); ++i) {
        const auto& xml = requestXmls[i];

        Transaction transaction;
        transaction.transNum = transNum++;

        // Extract request fields
        transaction.amount = extractXmlValue(xml, "ebl:OrderTotal");
        transaction.currency = extractXmlAttribute(xml, "currencyID");
        transaction.firstName = extractXmlValue(xml, "ebl:FirstName");
        transaction.lastName = extractXmlValue(xml, "ebl:LastName");
        transaction.street = extractXmlValue(xml, "ebl:Street1");
        transaction.city = extractXmlValue(xml, "ebl:CityName");
        transaction.state = extractXmlValue(xml, "ebl:StateOrProvince");
        transaction.zip = extractXmlValue(xml, "ebl:PostalCode");
        transaction.ccType = extractXmlValue(xml, "ebl:CreditCardType");
        transaction.ccLast4 = extractXmlValue(xml, "ebl:CreditCardLastFourDigits");
        transaction.expMonth = extractXmlValue(xml, "ebl:ExpMonth");
        transaction.expYear = extractXmlValue(xml, "ebl:ExpYear");
        transaction.cvv = extractXmlValue(xml, "ebl:CVV2");

        // Get corresponding response data
        if (i < responses.size()) {
            transaction.transId = responses[i].transId;
            transaction.status = responses[i].status;
            transaction.corrId = responses[i].corrId;
            transaction.procAmount = responses[i].procAmount;
        }

        transactions.push_back(transaction);
    }

    return transactions;
}

void outputRawData(const std::vector<Transaction>& transactions) {
    for (const auto& t : transactions) {
        std::cout << t.transNum << "|"
                  << t.amount << "|"
                  << t.currency << "|"
                  << t.firstName << "|"
                  << t.lastName << "|"
                  << t.street << "|"
                  << t.city << "|"
                  << t.state << "|"
                  << t.zip << "|"
                  << t.ccType << "|"
                  << t.ccLast4 << "|"
                  << t.expMonth << "|"
                  << t.expYear << "|"
                  << t.cvv << "|"
                  << t.transId << "|"
                  << t.status << "|"
                  << t.corrId << "|"
                  << t.procAmount << std::endl;
    }
}

void outputSummary(const std::vector<Transaction>& transactions) {
    std::cout << "=== SUMMARY ===" << std::endl;

    // Count transactions
    int total = transactions.size();
    int successful = std::count_if(transactions.begin(), transactions.end(),
                                  [](const Transaction& t) { return t.status == "Success"; });

    std::cout << "Total Transactions: " << total << std::endl;
    std::cout << "Successful: " << successful << std::endl;
    std::cout << "Failed: " << (total - successful) << std::endl;
    std::cout << std::endl;

    // Top 5 states
    std::map<std::string, int> stateCounts;
    for (const auto& t : transactions) {
        stateCounts[t.state]++;
    }

    std::cout << "Top 5 States by Transaction Count:" << std::endl;
    std::vector<std::pair<std::string, int>> stateCountVec(stateCounts.begin(), stateCounts.end());
    std::sort(stateCountVec.begin(), stateCountVec.end(),
             [](const auto& a, const auto& b) { return a.second > b.second; });

    int count = 0;
    for (const auto& sc : stateCountVec) {
        if (count++ >= 5) break;
        std::cout << "  " << sc.first << ": " << sc.second << std::endl;
    }
    std::cout << std::endl;

    // Transaction amount stats
    std::vector<double> amounts;
    for (const auto& t : transactions) {
        try {
            amounts.push_back(std::stod(t.amount));
        } catch (...) {
            // Skip invalid amounts
        }
    }

    if (!amounts.empty()) {
        double totalAmount = std::accumulate(amounts.begin(), amounts.end(), 0.0);
        double largest = *std::max_element(amounts.begin(), amounts.end());
        double smallest = *std::min_element(amounts.begin(), amounts.end());

        std::cout << "Transaction Amount Stats:" << std::endl;
        std::cout << "  Total: $" << std::fixed << std::setprecision(2) << totalAmount << std::endl;
        std::cout << "  Largest: $" << std::fixed << std::setprecision(2) << largest << std::endl;
        std::cout << "  Smallest: $" << std::fixed << std::setprecision(2) << smallest << std::endl;
    }
}
