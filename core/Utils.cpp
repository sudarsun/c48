#include "Utils.h"

const double Utils::SMALL = 1e-6;

bool Utils::isMissingValue(double val) {

    return std::isnan(val);
}

double Utils::missingValue() {

    return std::numeric_limits<double>::quiet_NaN();
}

double Utils::correlation(double_array &y1, double_array &y2, int n) {

    int i;
    double av1 = 0.0, av2 = 0.0, y11 = 0.0, y22 = 0.0, y12 = 0.0, c;

    if (n <= 1) {
        return 1.0;
    }
    for (i = 0; i < n; i++) {
        av1 += y1[i];
        av2 += y2[i];
    }
    av1 /= n;
    av2 /= n;
    for (i = 0; i < n; i++) {
        y11 += (y1[i] - av1) * (y1[i] - av1);
        y22 += (y2[i] - av2) * (y2[i] - av2);
        y12 += (y1[i] - av1) * (y2[i] - av2);
    }
    if (y11 * y22 == 0.0) {
        c = 1.0;
    }
    else {
        c = y12 / sqrt(abs(y11 * y22));
    }

    return c;
}

string Utils::removeSubstring(const string &inString, const string &substring) {

    string result = "";
    int oldLoc = 0, loc = 0;
    while ((loc = (int)inString.find(substring, oldLoc)) != -1) {
        result.append(inString.substr(oldLoc, loc - oldLoc));
        oldLoc = loc + (int)substring.length();
    }
    result.append(inString.substr(oldLoc));

    return result;
}

string Utils::replaceSubstring(const string &inString, const string &subString, const string &replaceString) {

    string result = "";
    int oldLoc = 0, loc = 0;
    while ((loc = (int)inString.find(subString, oldLoc)) != -1) {
        result.append(inString.substr(oldLoc, loc - oldLoc));
        result.append(replaceString);
        oldLoc = loc + (int)subString.length();
    }
    result.append(inString.substr(oldLoc));
    return result;
}

string Utils::doubleToString(double value, int afterDecimalPoint)
{
    string stringBuffer = "";
    double temp;
    int dotPosition;
    long precisionValue;
    temp = value * pow(10.0, afterDecimalPoint);
    if (abs(temp) < std::numeric_limits<long long>::max()) {
        precisionValue = (temp > 0) ? static_cast<long long>(temp + 0.5) : -static_cast<long long>(abs(temp) + 0.5);
        if (precisionValue == 0) {
            stringBuffer = std::to_string(0);
        }
        else {
            stringBuffer = std::to_string(precisionValue);
        }
        if (afterDecimalPoint == 0) {
            return stringBuffer;
        }
        dotPosition = (int)stringBuffer.length() - afterDecimalPoint;
        while (((precisionValue < 0) && (dotPosition < 1)) || (dotPosition < 0)) {
            if (precisionValue < 0) {
                stringBuffer.insert(1, "0");
            }
            else {
                stringBuffer.insert(0, "0");
            }
            dotPosition++;
        }
        stringBuffer.insert(dotPosition, ".");
        if ((precisionValue < 0) && (stringBuffer.at(1) == '.')) {
            stringBuffer.insert(1, "0");
        }
        else if (stringBuffer.at(0) == '.') {
            stringBuffer.insert(0, "0");
        }
        int currentPos = (int)stringBuffer.length() - 1;
        while ((currentPos > dotPosition) && (stringBuffer.at(currentPos) == '0')) {
            stringBuffer[currentPos--] = ' ';
        }
        if (stringBuffer.at(currentPos) == '.') {
            stringBuffer[currentPos] = ' ';
        }
        Utils::trim(stringBuffer);
        return stringBuffer;
    }
    return string(string("") + std::to_string(value));

}

string Utils::doubleToString(double value, int width, int afterDecimalPoint)
{

    string tempString = doubleToString(value, afterDecimalPoint);
    char_array result;
    int dotPosition;

    if (afterDecimalPoint >= width) {
        return tempString;
    }

    // Initialize result
    result = char_array(width);
    for (int i = 0; i < result.size(); i++) {
        result[i] = ' ';
    }

    if (afterDecimalPoint > 0) {
        // Get position of decimal point and insert decimal point
        dotPosition = (int)tempString.find(".");
        if (dotPosition == -1) {
            dotPosition = (int)tempString.length();
        }
        else {
            result[width - afterDecimalPoint - 1] = '.';
        }
    }
    else {
        dotPosition = (int)tempString.length();
    }

    int offset = width - afterDecimalPoint - dotPosition;
    if (afterDecimalPoint > 0) {
        offset--;
    }

    // Not enough room to decimal align within the supplied width
    if (offset < 0) {
        return tempString;
    }

    // Copy characters before decimal point
    for (int i = 0; i < dotPosition; i++) {
        result[offset + i] = tempString[i];
    }

    // Copy characters after decimal point
    for (int i = dotPosition + 1; i < tempString.length(); i++) {
        result[offset + i] = tempString[i];
    }

    return string(result.begin(), result.end());
}

bool Utils::eq(double a, double b)
{

    return (a == b) || ((a - b < SMALL) && (b - a < SMALL));
}

double Utils::info(int_array &counts)
{

    int total = 0;
    double x = 0;
    for (auto count : counts) {
        x -= xlogx(count);
        total += count;
    }
    return x + xlogx(total);
}

bool Utils::smOrEq(double a, double b)
{

    return (a - b < SMALL) || (a <= b);
}

bool Utils::grOrEq(double a, double b)
{

    return (b - a < SMALL) || (a >= b);
}

bool Utils::sm(double a, double b)
{

    return (b - a > SMALL);
}

bool Utils::gr(double a, double b)
{

    return (a - b > SMALL);
}

int Utils::kthSmallestValue(int_array &array_Renamed, int k) {

    int_array index = initialIndex((int)array_Renamed.size());
    return array_Renamed[index[select(array_Renamed, index, 0, (int)array_Renamed.size() - 1, k)]];
}

double Utils::kthSmallestValue(double_array &array_Renamed, int k) {

    int_array index = initialIndex((int)array_Renamed.size());
    return array_Renamed[index[select(array_Renamed, index, 0, (int)array_Renamed.size() - 1, k)]];
}

double Utils::getLog2(double a)
{

    return log(a) / log(2);
}

int Utils::maxIndex(double_array &doubles)
{

    double maximum = 0;
    int maxIndex = 0;

    for (int i = 0; i < doubles.size(); i++) {
        if ((i == 0) || (doubles[i] > maximum)) {
            maxIndex = i;
            maximum = doubles[i];
        }
    }

    return maxIndex;
}

int Utils::maxIndex(int_array &ints)
{

    int maximum = 0;
    int maxIndex = 0;

    for (int i = 0; i < ints.size(); i++) {
        if ((i == 0) || (ints[i] > maximum)) {
            maxIndex = i;
            maximum = ints[i];
        }
    }

    return maxIndex;
}

double Utils::mean(double_array &vector)
{

    double sum = 0;

    if (vector.empty()) {
        return 0;
    }
    for (auto element : vector) {
        sum += element;
    }
    return sum / vector.size();
}

int Utils::minIndex(int_array &ints)
{

    int minimum = 0;
    int minIndex = 0;

    for (int i = 0; i < ints.size(); i++) {
        if ((i == 0) || (ints[i] < minimum)) {
            minIndex = i;
            minimum = ints[i];
        }
    }

    return minIndex;
}

int Utils::minIndex(double_array &doubles)
{

    double minimum = 0;
    int minIndex = 0;

    for (int i = 0; i < doubles.size(); i++) {
        if ((i == 0) || (doubles[i] < minimum)) {
            minIndex = i;
            minimum = doubles[i];
        }
    }

    return minIndex;
}
void Utils::normalize(double_array &doubles) {

    double sum = 0;
    for (auto d : doubles) {
        sum += d;
    }
    normalize(doubles, sum);
}

void Utils::normalize(double_array &doubles, double sum) {

    if (std::isnan(sum)) {
        throw std::invalid_argument("Can't normalize array. Sum is NaN.");
    }
    if (sum == 0) {
        // Maybe this should just be a return.
        throw std::invalid_argument("Can't normalize array. Sum is zero.");
    }
    for (int i = 0; i < doubles.size(); i++) {
        doubles[i] /= sum;
    }
}

double_array Utils::logs2probs(double_array &a) {

    double max = a[maxIndex(a)];
    double sum = 0.0;

    double_array result(a.size());
    for (int i = 0; i < a.size(); i++) {
        result[i] = exp(a[i] - max);
        sum += result[i];
    }

    normalize(result, sum);

    return result;
}

double Utils::probToLogOdds(double prob)
{

    if (gr(prob, 1) || (sm(prob, 0))) {
        throw std::invalid_argument(string("probToLogOdds: probability must ") + string("be in [0,1] ") + std::to_string(prob));
    }
    double p = SMALL + (1.0 - 2 * SMALL) * prob;
    return log(p / (1 - p));
}

int Utils::round(double value)
{

    int roundedValue = value > 0 ? static_cast<int>(value + 0.5) : -static_cast<int>(abs(value) + 0.5);

    return roundedValue;
}

/*int Utils::probRound( double value, Random *rand ) {

  if( value >= 0 ) {
  double lower = floor( value );
  double prob = value - lower;
  if( rand->nextDouble() < prob ) {
  return static_cast<int>( lower ) + 1;
  } else {
  return static_cast<int>( lower );
  }
  } else {
  double lower = floor( abs( value ) );
  double prob = abs( value ) - lower;
  if( rand->nextDouble() < prob ) {
  return -( static_cast<int>( lower ) + 1 );
  } else {
  return -static_cast<int>( lower );
  }
  }
  }*/

void Utils::replaceMissingWithMAX_VALUE(double_array &array_Renamed) {

    for (int i = 0; i < array_Renamed.size(); i++) {
        if (isMissingValue(array_Renamed[i])) {
            array_Renamed[i] = std::numeric_limits<double>::max();
        }
    }
}

double Utils::roundDouble(double value, int afterDecimalPoint)
{

    double mask = pow(10.0, afterDecimalPoint);

    return (round(value * mask)) / mask;
}

int_array Utils::Sort(int_array &array_Renamed)
{

    int_array index = initialIndex((int)array_Renamed.size());
    int_array newIndex(array_Renamed.size());
    int_array helpIndex;
    int numEqual;

    quickSort(array_Renamed, index, 0, (int)array_Renamed.size() - 1);

    // Make sort stable
    int i = 0;
    while (i < index.size()) {
        numEqual = 1;
        for (int j = i + 1; ((j < index.size()) && (array_Renamed[index[i]] == array_Renamed[index[j]])); j++) {
            numEqual++;
        }
        if (numEqual > 1) {
            helpIndex = int_array(numEqual);
            for (int j = 0; j < numEqual; j++) {
                helpIndex[j] = i + j;
            }
            quickSort(index, helpIndex, 0, numEqual - 1);
            for (int j = 0; j < numEqual; j++) {
                newIndex[i + j] = index[helpIndex[j]];
            }
            i += numEqual;
        }
        else {
            newIndex[i] = index[i];
            i++;
        }
    }
    return newIndex;
}

int_array Utils::Sort(double_array &array_Renamed)
{ // @non_null@ -  @pure@

    int_array index = initialIndex((int)array_Renamed.size());
    if (array_Renamed.size() > 1) {
        //array_Renamed = array_Renamed;
        replaceMissingWithMAX_VALUE(array_Renamed);
        quickSort(array_Renamed, index, 0, (int)array_Renamed.size() - 1);
    }
    return index;
}

int_array Utils::sortWithNoMissingValues(double_array &array_Renamed)
/* @non_null@ */
{

    int_array index = initialIndex((int)array_Renamed.size());
    if (array_Renamed.size() > 1) {
        quickSort(array_Renamed, index, 0, (int)array_Renamed.size() - 1);
    }
    return index;
}

int_array Utils::stableSort(double_array &array_Renamed)
{

    int_array index = initialIndex((int)array_Renamed.size());

    if (array_Renamed.size() > 1) {

        int_array newIndex(array_Renamed.size());
        int_array helpIndex;
        int numEqual;

        //array_Renamed = array_Renamed.clone();
        replaceMissingWithMAX_VALUE(array_Renamed);
        quickSort(array_Renamed, index, 0, (int)array_Renamed.size() - 1);

        // Make sort stable

        int i = 0;
        while (i < index.size()) {
            numEqual = 1;
            for (int j = i + 1; ((j < index.size()) && Utils::eq(array_Renamed[index[i]], array_Renamed[index[j]])); j++) {
                numEqual++;
            }
            if (numEqual > 1) {
                helpIndex = int_array(numEqual);
                for (int j = 0; j < numEqual; j++) {
                    helpIndex[j] = i + j;
                }
                quickSort(index, helpIndex, 0, numEqual - 1);
                for (int j = 0; j < numEqual; j++) {
                    newIndex[i + j] = index[helpIndex[j]];
                }
                i += numEqual;
            }
            else {
                newIndex[i] = index[i];
                i++;
            }
        }
        return newIndex;
    }
    else {
        return index;
    }
}

double Utils::variance(double_array &vector)
{

    if (vector.size() <= 1) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    double mean = 0;
    double var = 0;

    for (int i = 0; i < vector.size(); i++) {
        double delta = vector[i] - mean;
        mean += delta / (i + 1);
        var += (vector[i] - mean)*delta;
    }

    var /= vector.size() - 1;

    // We don't like negative variance
    if (var < 0) {
        return 0;
    }
    else {
        return var;
    }
}

double Utils::sum(double_array &doubles)
{

    double sum = 0;

    for (auto d : doubles) {
        sum += d;
    }
    return sum;
}

int Utils::sum(int_array &ints)
{

    int sum = 0;

    for (auto j : ints) {
        sum += j;
    }
    return sum;
}

double Utils::xlogx(int c)
{

    if (c == 0) {
        return 0.0;
    }
    return c * Utils::getLog2(c);
}

int_array Utils::initialIndex(int size) {

    int_array index(size);
    for (int i = 0; i < size; i++) {
        index[i] = i;
    }
    return index;
}

int Utils::sortLeftRightAndCenter(double_array &array_Renamed, int_array &index, int l, int r) {

    int c = (l + r) / 2;
    conditionalSwap(array_Renamed, index, l, c);
    conditionalSwap(array_Renamed, index, l, r);
    conditionalSwap(array_Renamed, index, c, r);
    return c;
}

void Utils::swap(int_array &index, int l, int r) {

    int help = index[l];
    index[l] = index[r];
    index[r] = help;
}

void Utils::conditionalSwap(double_array &array_Renamed, int_array &index, int left, int right) {

    if (array_Renamed[index[left]] > array_Renamed[index[right]]) {
        int help = index[left];
        index[left] = index[right];
        index[right] = help;
    }
}

int Utils::partition(double_array &array_Renamed, int_array &index, int l, int r, double pivot) {

    r--;
    while (true) {
        while ((array_Renamed[index[++l]] < pivot)) {
            ;
        }
        while ((array_Renamed[index[--r]] > pivot)) {
            ;
        }
        if (l >= r) {
            return l;
        }
        swap(index, l, r);
    }
}

int Utils::partition(int_array &array_Renamed, int_array &index, int l, int r) {

    double pivot = array_Renamed[index[(l + r) / 2]];
    int help;

    while (l < r) {
        while ((array_Renamed[index[l]] < pivot) && (l < r)) {
            l++;
        }
        while ((array_Renamed[index[r]] > pivot) && (l < r)) {
            r--;
        }
        if (l < r) {
            help = index[l];
            index[l] = index[r];
            index[r] = help;
            l++;
            r--;
        }
    }
    if ((l == r) && (array_Renamed[index[r]] > pivot)) {
        r--;
    }

    return r;
}

void Utils::quickSort(double_array &array_Renamed, int_array &index, int left, int right)
{ // @non_null@ -  @non_null@

    int diff = right - left;

    switch (diff) {
    case 0:

        // No need to do anything
        return;
    case 1:

        // Swap two elements if necessary
        conditionalSwap(array_Renamed, index, left, right);
        return;
    case 2:

        // Just need to sort three elements
        conditionalSwap(array_Renamed, index, left, left + 1);
        conditionalSwap(array_Renamed, index, left, right);
        conditionalSwap(array_Renamed, index, left + 1, right);
        return;
    default:

        // Establish pivot
        int pivotLocation = sortLeftRightAndCenter(array_Renamed, index, left, right);

        // Move pivot to the right, partition, and restore pivot
        swap(index, pivotLocation, right - 1);
        int center = partition(array_Renamed, index, left, right, array_Renamed[index[right - 1]]);
        swap(index, center, right - 1);

        // Sort recursively
        quickSort(array_Renamed, index, left, center - 1);
        quickSort(array_Renamed, index, center + 1, right);
    }
}

void Utils::quickSort(int_array &array_Renamed, int_array &index, int left, int right)
{ // @non_null@ -  @non_null@

    if (left < right) {
        int middle = partition(array_Renamed, index, left, right);
        quickSort(array_Renamed, index, left, middle);
        quickSort(array_Renamed, index, middle + 1, right);
    }
}

int Utils::select(double_array &array_Renamed, int_array &index, int left, int right, int k)
{ // @non_null@ -  @non_null@

    int diff = right - left;
    switch (diff) {
    case 0:

        // Nothing to be done
        return left;
    case 1:

        // Swap two elements if necessary
        conditionalSwap(array_Renamed, index, left, right);
        return left + k - 1;
    case 2:

        // Just need to sort three elements
        conditionalSwap(array_Renamed, index, left, left + 1);
        conditionalSwap(array_Renamed, index, left, right);
        conditionalSwap(array_Renamed, index, left + 1, right);
        return left + k - 1;
    default:

        // Establish pivot
        int pivotLocation = sortLeftRightAndCenter(array_Renamed, index, left, right);

        // Move pivot to the right, partition, and restore pivot
        swap(index, pivotLocation, right - 1);
        int center = partition(array_Renamed, index, left, right, array_Renamed[index[right - 1]]);
        swap(index, center, right - 1);

        // Proceed recursively
        if ((center - left + 1) >= k) {
            return select(array_Renamed, index, left, center, k);
        }
        else {
            return select(array_Renamed, index, center + 1, right, k - (center - left + 1));
        }
    }
}

int Utils::select(int_array &array_Renamed, int_array &index, int left, int right, int k)
{ // @non_null@ -  @non_null@

    if (left == right) {
        return left;
    }
    else {
        int middle = partition(array_Renamed, index, left, right);
        if ((middle - left + 1) >= k) {
            return select(array_Renamed, index, left, middle, k);
        }
        else {
            return select(array_Renamed, index, middle + 1, right, k - (middle - left + 1));
        }
    }
}

string Utils::backQuoteChars(string inString)
{

    int index;
    string stringbuf;

    // replace each of the following characters with the backquoted version
    char_array charsFind = { '\\', '\'', '\t', '\n', '\r', '"', '%', '\u001E' };
    string_array charsReplace = { "\\\\", "\\'", "\\t", "\\n", "\\r", "\\\"", "\\%", "\\u001E" };
    for (int i = 0; i < charsFind.size(); i++) {
        if (inString.find(charsFind[i]) != -1) {
            stringbuf = "";
            while ((index = (int)inString.find(charsFind[i])) != -1) {
                if (index > 0) {
                    stringbuf.append(inString.substr(0, index));
                }
                stringbuf.append(charsReplace[i]);
                if ((index + 1) < inString.length()) {
                    inString = inString.substr(index + 1);
                }
                else {
                    inString = "";
                }
            }
            stringbuf.append(inString);
        }
    }

    return stringbuf;
}

void Utils::trim(string &outString)
{
    outString.erase(outString.find_last_not_of(' ') + 1);
    outString.erase(0, outString.find_first_not_of(' '));
}
