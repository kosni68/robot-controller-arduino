
void overflow(byte &value_actuel, byte value_min, byte value_max)
{
    if (value_actuel < value_min)
        value_actuel = value_max;

    if (value_actuel > value_max)
        value_actuel = value_min;
}