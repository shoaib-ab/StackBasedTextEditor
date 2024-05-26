#include <iostream>
#include <stack>
#include <string>
using namespace std;

class SimpleTextEditor {
private:
    string text;
    int cursorPosition;
    stack<pair<string, int>> undoStack;
    stack<pair<string, int>> redoStack;

public:
    SimpleTextEditor() : cursorPosition(0) {}

    void insertText(const string& str) {
        text.insert(cursorPosition, str);
        undoStack.push({ str, cursorPosition });
        while (!redoStack.empty()) redoStack.pop();
        cursorPosition += str.size();
    }

    void deleteCharacter() {
        if (cursorPosition > 0) {
            char deletedChar = text[cursorPosition - 1];
            text.erase(cursorPosition - 1, 1);
            undoStack.push({ string(1, deletedChar), -cursorPosition });
            while (!redoStack.empty()) redoStack.pop();
            cursorPosition--;
        }
    }

    void moveCursorTo(int pos) {
        if (pos >= 0 && pos <= text.size()) {
            cursorPosition = pos;
        }
    }

    void moveCursorNext() {
        if (cursorPosition < text.size()) {
            cursorPosition++;
        }
    }

    void moveCursorPrevious() {
        if (cursorPosition > 0) {
            cursorPosition--;
        }
    }

    void moveCursorToStart() {
        cursorPosition = 0;
    }

    void moveCursorToEnd() {
        cursorPosition = text.size();
    }

    void undoAction() {
        if (!undoStack.empty()) {
            auto lastAction = undoStack.top();
            undoStack.pop();
            if (lastAction.second >= 0) {
                text.erase(lastAction.second, lastAction.first.size());
                cursorPosition = lastAction.second;
            }
            else {
                text.insert(-lastAction.second - 1, lastAction.first);
                cursorPosition = -lastAction.second;
            }
            redoStack.push(lastAction);
        }
    }

    void redoAction() {
        if (!redoStack.empty()) {
            auto lastAction = redoStack.top();
            redoStack.pop();
            if (lastAction.second >= 0) {
                text.insert(lastAction.second, lastAction.first);
                cursorPosition = lastAction.second + lastAction.first.size();
            }
            else {
                text.erase(-lastAction.second - 1, lastAction.first.size());
                cursorPosition = -lastAction.second - 1;
            }
            undoStack.push(lastAction);
        }
    }

    void displayText() const {
        cout << text << endl;
        for (int i = 0; i < cursorPosition; i++) cout << " ";
        cout << "^" << endl;
    }

    int getCursorPosition() const {
        return cursorPosition;
    }
};

int main() {
    SimpleTextEditor editor;
    string command;
    cout << "Simple Text Editor\n";
    cout << "Commands:\n";
    cout << "i <string> : Insert text\n";
    cout << "d : Delete character\n";
    cout << "u : Undo\n";
    cout << "r : Redo\n";
    cout << "n : Move cursor next (right)\n";
    cout << "p : Move cursor previous (left)\n";
    cout << "s : Move cursor to start\n";
    cout << "e : Move cursor to end\n";
    cout << "c : Display current cursor position\n";
    cout << "q : Quit\n";

    while (true) {
        editor.displayText();
        cout << "Enter command: ";
        cin >> command;

        if (command == "i") {
            string str;
            cin.ignore();
            getline(cin, str);
            editor.insertText(str);
        }
        else if (command == "d") {
            editor.deleteCharacter();
        }
        else if (command == "u") {
            editor.undoAction();
        }
        else if (command == "r") {
            editor.redoAction();
        }
        else if (command == "n") {
            editor.moveCursorNext();
        }
        else if (command == "p") {
            editor.moveCursorPrevious();
        }
        else if (command == "s") {
            editor.moveCursorToStart();
        }
        else if (command == "e") {
            editor.moveCursorToEnd();
        }
        else if (command == "c") {
            cout << "Current cursor position: " << editor.getCursorPosition() << endl;
        }
        else if (command == "q") {
            break;
        }
        else {
            cout << "Invalid command!" << endl;
        }
    }

    return 0;
}
