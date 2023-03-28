# Альтернативное задание для лабораторной работы 1

## Задача:
Разработать драйвер символьного устройсва для одноплатного компьюьтера Raspberry PI, управляющий компьютерным вентилятором с помощью ШИМ.
Разработать программу, работающую на пользовательском уровне и осуществляющую взаимодействие с драйвером.
Собрать демонстрационную схему и продемонстрировать работу.

[Демонстрация](https://youtu.be/ez0I1_m163k)

## Скрипты для теста
В каталоге scripts содердится 3 bash сценария:
- `build_install.sh` должен запускаться с sudo - собирает драйвер и пользовательскую программу, устанавливает драйвер
- `test.sh` проводит тестированеи драйвера. Устанавливает разную мощность с помощью write и проверяет корректность результата read
- `driver_remove`  должен запускаться с sudo - извлекает драйвер

## Интерфейс драйвера
Разработанный драйвер поддерживает стандартные file_operations: open, read, write, close. Операции open, close используют атомарную переменную для гарантии того, что только один поток использует устройство в каждый момент времени.

Операция write читает один байт из пользовательского буфера и интерпретирует его как число от 0 до 255 - скорость вращения вентилятора в условных единицах.

Операция read перемещает в пользовательский буфер 1 байт, интерпретируемый как число от 0 до 255 - скорость вращения вентилятора в условных единицах.

## Программа пользовательского уровня
Разработанная программа принимает 2 типа команд аргументами командной строки. 

Команда `read` не требует аргументов и печатает пользователю число от 0 до 255 - текущую скорость вращения вентилятора.

Команда `write` принимает один аргумент - число от 0 до 255 - скорость, которую требуется установить вентилятору.

Программа умеет обрабатывать ошибки ввода: неправильная команда, неправильный аргумент, нет команды. 
Также проверяется наличие драйвера. 
Если нужное устройство не находится в директории `dev`, пользователю выдаётся сообщениет об ошибке.

Исходный код программы располагается в каталоге `user` данного репозитория.
Для сборки из каталога `user` необходимо выполнить команду make.

## Аспекты реализации драйвера
Raspberry PI 3B+ оснащён процессором Broadcom BCM2837. Он имеет специальный блок регистров для управления портами GPIO, а также для управления двумя ШИМ каналами. Главный "трюк" заключается в том, чтобы описать нужные блоки регистров структурами, а конкретные регистры битовыми полями, а потом отобразить указатели на эти структуры в адреса памяти, которые процессор BCM2837 использует для доступа к интересующим нас регистрам.

Приведём пример такой структуры и битового поля:
```
volatile struct S_PWM_REGS
{
    uint32_t CTL;
    uint32_t STA;
    uint32_t DMAC;
    uint32_t reserved0;
    uint32_t RNG1;
    uint32_t DAT1;
    uint32_t FIF1;
    uint32_t reserved1;
    uint32_t RNG2;
    uint32_t DAT2;
} *pwm_regs;

volatile struct S_PWM_CTL {
    unsigned PWEN1 : 1;
    unsigned MODE1 : 1;
    unsigned RPTL1 : 1;
    unsigned SBIT1 : 1;
    unsigned POLA1 : 1;
    unsigned USEF1 : 1;
    unsigned CLRF1 : 1;
    unsigned MSEN1 : 1;
    unsigned PWEN2 : 1;
    unsigned MODE2 : 1;
    unsigned RPTL2 : 1;
    unsigned SBIT2 : 1;
    unsigned POLA2 : 1;
    unsigned USEF2 : 1;
    unsigned Reserved1 : 1;
    unsigned MSEN2 : 1;
    unsigned Reserved2 : 16;
} *pwm_ctl;
```

Обратите внимание на использование ключевого слова `volatile`. Оно необходимо, чтобы любое обращение по указателю в программе действитель приводило к обращению по соответсвующему адресу памяти. Без него значения структуры могут оставаться в регистрах вычислительного ядра. В данном случае это напрямую влияет на то, будут ли обновлены значения управляющих регистров или нет.

Отображение структур на адрес происходит следующим образом:
```
gpio_regs = (struct S_GPIO_REGS *)ioremap(GPIO_BASE, sizeof(struct S_GPIO_REGS));
pwm_regs = (struct S_PWM_REGS *)ioremap(PWM_BASE, sizeof(struct S_PWM_REGS));
```

При выгрузке драйвера необходимо удалить отображение:
```
iounmap(gpio_regs);
iounmap(pwm_regs);
```

Далее требуется описать удобные обёртки для установки частоты и скважности ШИМ.
```
static void pwm_ratio(unsigned n, unsigned m) {

    // Disable PWM Channel 2
    pwm_ctl->PWEN2 = 0;

    // Set the PWM Channel 2 Range Register
    pwm_regs->RNG2 = m;
    // Set the PWM Channel 2 Data Register
    pwm_regs->DAT2 = n;

    // Check if PWM Channel 2 is not currently transmitting
    if ( !pwm_sta->STA2 ) {
        if ( pwm_sta->RERR1 ) pwm_sta->RERR1 = 1; // Clear RERR bit if read occured on empty FIFO while channel was transmitting
        if ( pwm_sta->WERR1 ) pwm_sta->WERR1 = 1; // Clear WERR bit if write occured on full FIFO while channel was transmitting
        if ( pwm_sta->BERR ) pwm_sta->BERR = 1; // Clear BERR bit if write to registers via APB occured while channel was transmitting
    }
    udelay(10);

    // Enable PWM Channel 2
    pwm_ctl->PWEN2 = 1;
}
```

После этого задача сводится к тривиальной: преобразовывать ввод в скважность и устанавливать её.

## Позитивные черты работы
В отличие от стандартного задания первой лабораторной по Системам вводв-вывода, данное упражнение заставляет действительно изучать документацию на аппаратные компоненты вычислительной системы и бороться с проблемами, возникающими при взаимодействии софта с реальным миром.
Студенты, выполнявшие это задание, на практике ощутили сложности разработки и отладки драйвера физического устройства. Кроме того, были получены навыки администрирования Raspbery PI.

## Предложения по доработке
Компьютерный вентилятор при управлении через ШИМ не может устанавливаться. При подаче 0 на ШИМ вход он продолжает вращаться с минимальной скоростью.
Чтобы добавить возможность остановки при подаче нулевой скорости, можно добавить в схему полевой транзистор, разрывающий питание вентилятора, повесить его на ещё один GPIO и модифицировать управляющую логику драйвера.

Также имеет смысл добавить драйверу поддержку интерфейса `ioctl`

Для отладки можно жобавить журнал событий, который может выводиться при чтении файла утсройства в procfs
