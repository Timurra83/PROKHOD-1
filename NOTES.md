# Дневник разработки ПРОХОД-1

## 2026-07-01
- Попытка сделать макет второй версии на текстолите "мертвым жуком"
- Потеря стабилизатора-сгорел. побит вход-земля
По какой то причине после подключения есп32 напряжение после стабилизатора проседает до 6 вольт вместо 12, начинается паразитная генерация. Был настроен DC-DC преобразователь
## 2026-07-02
- Разбор проблемы с драйвером
- *ЭКСПЕРИМЕНТ:* Установка преобразователя 24-12 вольт для питания драйвера от источника питания
- *СЛЕДСТВИЕ* слияние сигнала на драйвере с 7 до 3.5мгц
- потеря ESP32. предположительно из за перенапряжения, сомнительно
- раздумья о переходе на кварцевый генератор
- успех на данном этапе отсутствует
## 2026-07-02
- Заказаны запчасти для генератора
- Создана тестовая плата для генератора

<img width="326" height="491" alt="проба_генератора" src="https://github.com/user-attachments/assets/7e2ffb3b-3355-4760-8df9-ff66adb06ebd" />


## 2026-07-07
- создан внешний переключаемый фнч на 7 и 14мгц

## 2026-10-17
- создана схема усилителя Е класса
<img width="1410" height="437" alt="ob_nRxBAPlznsmBBmo595xdrLZL8nT6FSfbiTSLedrZoUWSKNwbQLUgxBWNqFMzVCD6BrEOcZuEIFtdRg6KWgyHO" src="https://github.com/user-attachments/assets/ad2df245-1a87-4644-9289-6111e39a31f2" />

- проведены симуляции 
<img width="1495" height="1040" alt="V_drain" src="https://github.com/user-attachments/assets/07df23c4-3723-4941-a842-5254e4969538" />
<img width="1495" height="754" alt="Spectrum" src="https://github.com/user-attachments/assets/cb839e91-2140-4ef2-ac8f-20015d719d82" />

## 2026-10-18
- перерисована схема в изи идея
<img width="1821" height="620" alt="image" src="https://github.com/user-attachments/assets/57c990a7-cbf2-4b32-80bc-4868450f6cb8" />

- л контур заменен на п контур
<img width="620" height="319" alt="image" src="https://github.com/user-attachments/assets/fecbf47e-5f5b-4fca-92e7-f5f12822c847" />

- проведено множество исследований схемы в симуляции
<img width="620" height="248" alt="image" src="https://github.com/user-attachments/assets/e7a3e982-3484-4b70-b131-a4c97f02ebd5" />
<img width="586" height="407" alt="image" src="https://github.com/user-attachments/assets/55212ace-b386-409e-ae1d-2cc6f29e51f2" />

- тест ZVS
<img width="877" height="183" alt="image" src="https://github.com/user-attachments/assets/23fc0cbc-3a35-42c6-8b1d-4251dd2a55b9" />

- теоретические энергетические параметры
<img width="849" height="249" alt="image" src="https://github.com/user-attachments/assets/979025ea-0544-494a-a15a-f7a7e0b089b4" />

- тепловые расчеты: Температура перехода считается самосогласованно: сопротивление канала растёт с температурой (около 0.8 %/°C), поэтому потери и нагрев взаимозависимы. Уравнение Tj = Ta + (I_rms²·Rds(on)(Tj) + P_проч)·θ решается итерациями.
<img width="865" height="181" alt="image" src="https://github.com/user-attachments/assets/c266e673-9b5e-4b87-8351-16f9fbfce1e6" />

<img width="968" height="530" alt="image" src="https://github.com/user-attachments/assets/e112ff32-485f-48b5-b9fb-d63dd874a100" />

- скважность сигнала
<img width="968" height="530" alt="image" src="https://github.com/user-attachments/assets/71638e58-cc7f-4c55-8796-8477102feb98" />

-Заметка изменения: П-контур подавляет гармоники именно тем, что сильнее их шунтирует. Это дополнительно нагружает сток на гармониках и заостряет вершину импульса: пик вырос с 91.1 до 95.4 В. Симуляция выполнена при 27 °C; реальный КПД при прогретом приборе ниже (оценка 86…88 % по горячему расчёту).
-Давно, но создан генератор на кварце. Точный сигнал неизвестен из за недостатка прибора
<img width="2560" height="1920" alt="image" src="https://github.com/user-attachments/assets/130f5b75-438d-4f7e-9c79-c6306662f0fd" />





