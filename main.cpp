#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stack>

class Text {
public:
    Text() {
        currentLine = 0;
        undoStack.push_back(lines);
        maxHistorySize = 3;
    }

    void append(const std::string &text) {
        if (currentLine >= lines.size()) {
            lines.push_back(text);
        } else {
            lines[currentLine] += "\n" + text;
        }
        updateHistory();
    }

    void newLine() {
        currentLine++;
        lines.push_back("");
        updateHistory();
    }

    void insert(int lineIndex, int symbolIndex, const std::string &text, bool replace) {
        if (lineIndex >= 0 && lineIndex < lines.size() && symbolIndex >= 0) {
            if (lineIndex < lines.size() && symbolIndex <= lines[lineIndex].size()) {
                std::string &line = lines[lineIndex];
                if (replace) {
                    if (symbolIndex + text.length() <= line.length()) {
                        line.replace(symbolIndex, text.length(), text);
                    } else {
                        std::cout << "Replacement text exceeds line boundary" << std::endl;
                        return;
                    }
                } else {
                    line.insert(symbolIndex, text);
                }
                updateHistory();
            } else {
                std::cout << "Symbol index is out of range" << std::endl;
            }
        } else {
            std::cout << "Invalid line index" << std::endl;
        }
    }

    void deleteText(int lineIndex, int symbolIndex, int numSymbols) {
        if (lineIndex >= 0 && lineIndex < lines.size() && symbolIndex >= 0) {
            std::string &line = lines[lineIndex];
            if (symbolIndex < line.size()) {
                line.erase(symbolIndex, numSymbols);
                updateHistory();
            } else {
                std::cout << "Symbol index is out of range" << std::endl;
            }
        } else {
            std::cout << "Invalid line index" << std::endl;
        }
    }

    void clear() {
        lines.clear();
        currentLine = 0;
        updateHistory();
    }

    void undo() {
        if (undoStack.size() > 1) {
            redoStack.push_back(undoStack.back());
            undoStack.pop_back();
            lines = undoStack.back();
        }
    }

    void redo() {
        if (!redoStack.empty()) {
            undoStack.push_back(redoStack.back());
            redoStack.pop_back();
            lines = undoStack.back();
        }
    }

    const std::vector<std::string>& getLines() const {
        return lines;
    }

private:
    std::vector<std::string> lines;
    int currentLine;
    std::vector<std::vector<std::string>> undoStack;
    std::vector<std::vector<std::string>> redoStack;
    int maxHistorySize;

    void updateHistory() {
        undoStack.push_back(lines);
        if (undoStack.size() > maxHistorySize) {
            undoStack.erase(undoStack.begin());
        }
        redoStack.clear();
    }
};

class FileManager {
public:
    static void saveToFile(const std::string &fileName, const Text &text) {
        std::ofstream file(fileName);
        if (file.is_open()) {
            for (const auto &line : text.getLines()) {
                file << line << '\n';
            }
            file.close();
            std::cout << "Text has been saved successfully" << std::endl;
        } else {
            std::cout << "Error opening file" << std::endl;
        }
    }

    static void loadFromFile(const std::string &fileName, Text &text) {
        text.clear();

        std::ifstream file(fileName);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                text.append(line);
            }
            file.close();
            std::cout << "Text has been loaded successfully" << std::endl;
        } else {
            std::cout << "Error opening file" << std::endl;
        }
    }
};

class TextEditor {
public:
    void printText(const Text &text) {
        std::cout << "Current Text:" << std::endl;
        const auto &lines = text.getLines();
        for (const auto &line : lines) {
            std::cout << line << '\n';
        }
    }

    void searchText(const Text &text, const std::string &search) {
        std::cout << "Text is present in these positions:" << std::endl;
        const auto &lines = text.getLines();
        for (int i = 0; i < lines.size(); i++) {
            const std::string &line = lines[i];
            size_t position = 0;
            while ((position = line.find(search, position)) != std::string::npos) {
                std::cout << "Line " << i << ", Symbol " << position << std::endl;
                position += search.length();
            }
        }
    }
};

int main() {
    Text text;
    TextEditor editor;
    FileManager fileManager;

    while (true) {
        std::cout << "Choose the command:" << std::endl;
        std::cout << "1. Enter text to append" << std::endl;
        std::cout << "2. Start a new line" << std::endl;
        std::cout << "3. Save text to a file" << std::endl;
        std::cout << "4. Load text from a file" << std::endl;
        std::cout << "5. Print current text" << std::endl;
        std::cout << "6. Insert text by line and symbol index" << std::endl;
        std::cout << "7. Search for text" << std::endl;
        std::cout << "8. Delete text" << std::endl;
        std::cout << "9. Undo" << std::endl;
        std::cout << "10. Redo" << std::endl;
        std::cout << "14. Insert text with replacement" << std::endl;
        std::cout << "20. Clear the text" << std::endl;

        int choice;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                std::cin.ignore();
                std::string input;
                std::cout << "Enter text to append: ";
                std::getline(std::cin, input);
                text.append(input);
                std::cout << "Text appended: " << input << std::endl;
                break;
            }
            case 2: {
                text.newLine();
                std::cout << "New line is started" << std::endl;
                break;
            }
            case 3: {
                std::string fileName;
                std::cout << "Enter the file name for saving: ";
                std::cin >> fileName;
                fileManager.saveToFile(fileName, text);
                break;
            }
            case 4: {
                std::string fileName;
                std::cout << "Enter the file name for loading: ";
                std::cin >> fileName;
                fileManager.loadFromFile(fileName, text);
                break;
            }
            case 5: {
                editor.printText(text);
                break;
            }
            case 6: {
                int lineIndex, symbolIndex;
                std::cout << "Choose line and symbol index: ";
                std::cin >> lineIndex >> symbolIndex;
                std::cin.ignore(); // Clear the input buffer
                std::string input;
                std::cout << "Enter text to insert: ";
                std::getline(std::cin, input);
                text.insert(lineIndex, symbolIndex, input, false);
                break;
            }
            case 7: {
                std::cin.ignore();
                std::string search;
                std::cout << "Enter text to search: ";
                std::getline(std::cin, search);
                editor.searchText(text, search);
                break;
            }
            case 8: {
                int lineIndex, symbolIndex, numSymbols;
                std::cout << "Choose line, index, and number of symbols to delete: ";
                std::cin >> lineIndex >> symbolIndex >> numSymbols;
                text.deleteText(lineIndex, symbolIndex, numSymbols);
                break;
            }
            case 9: {
                text.undo();
                std::cout << "Undo performed" << std::endl;
                break;
            }
            case 10: {
                text.redo();
                std::cout << "Redo performed" << std::endl;
                break;
            }
            case 14: {
                int lineIndex, symbolIndex;
                std::cout << "Choose line and symbol index: ";
                std::cin >> lineIndex >> symbolIndex;
                std::cin.ignore(); // Clear the input buffer
                std::string input;
                std::cout << "Enter text to insert (with replacement): ";
                std::getline(std::cin, input);
                text.insert(lineIndex, symbolIndex, input, true);
                break;
            }
            case 20: {
                text.clear();
                std::cout << "Text cleared" << std::endl;
                break;
            }
            default: {
                std::cout << "Invalid choice" << std::endl;
            }
        }
    }

    return 0;
}
