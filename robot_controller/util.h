
void overflow(byte &value_actuel, byte value_min, byte value_max)
{
    if (value_actuel < value_min)
        value_actuel = value_max;

    if (value_actuel > value_max)
        value_actuel = value_min;
}

int min_max(int value_actuel, int value_min, int value_max)
{
    if (value_actuel < value_min)
        value_actuel = value_min;

    if (value_actuel > value_max)
        value_actuel = value_max;

    return value_actuel;
}

void soft_restart()
{

    Serial.println(F("RESTART"));
    delay(1000);
    asm volatile("  jmp 0");
}