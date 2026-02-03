#include <iostream>

void choiceFunction(int choice);
int main() {
#ifdef DESKTOP
  std::cout << "Press 1 to play Tetris (cli), 2 to play Snake (cli), 3 to play "
               "Tetris (desktop), 4 to play Snake (desktop), 5 - Exit "
            << std::endl;
#endif
#ifdef CLI
  std::cout << "Press 1 to play Tetris (cli), 2 to play Snake (cli), 5 - Exit "
            << std::endl;
#endif
  int choice;

  while (true) {
    std::cout << "Your choice: ";
    if (!(std::cin >> choice)) {
      std::cin.clear();
      std::cin.ignore(10000, '\n');
      std::cout << "Invalid input. Try again.\n";
      continue;
    }
#ifdef DESKTOP
    if (choice == 1 || choice == 2 || choice == 3 || choice == 4) {
      choiceFunction(choice);
    }
#endif
    if (choice == 1 || choice == 2) {
      choiceFunction(choice);
    } else if (choice == 5) {
      break;
    } else {
      std::cout << "Invalid option. Try again.\n";
    }
  }
  return 0;
}

void choiceFunction(int choice) {
  switch (choice) {
    case 1:
      system("tetris");
      break;
    case 2:
      system("snake");
      break;
#ifdef DESKTOP
    case 3:
      system("desktop_tetris.exe");
      break;
    case 4:
      system("snake_gui.exe");
      break;
#endif
  }
}