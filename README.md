Приложение калькулятор использует библиотеку mathlib

# Запуск службы после сборки и установки
sudo systemctl daemon-reload
sudo systemctl enable calculator.service
sudo systemctl start calculator.service

# Проверка
./test_client '{"operation":"+","operand1":10,"operand2":5}'
# Должен вывести 15

### Примеры использования
# Сложение
./test_client '{"operation":"+","operand1":15,"operand2":25}'

# Вычитание
./test_client '{"operation":"-","operand1":100,"operand2":45}'

# Умножение
./test_client '{"operation":"*","operand1":12,"operand2":11}'

# Деление
./test_client '{"operation":"/","operand1":144,"operand2":12}'

# Возведение в степень
./test_client '{"operation":"^","operand1":3,"operand2":4}'

# Факториал
./test_client '{"operation":"!","operand":5}'

