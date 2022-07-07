const tempValueSpan = document.getElementById('tempValue');
const tempUnitSpan = document.getElementById('tempUnit');
const humValueSpan = document.getElementById('humValue');
const humUnitSpan = document.getElementById('humUnit');

setInterval(async () => 
{
    const json = await fetch('http://192.168.0.104').then(res => res.json());

    const { temperature, humidity } = json || {
        temperature:
        {
            value: 42,
            unit: '°C'
        },
        humidity:
        {
            value: 20,
            unit: '%'
        }
    };

    tempValueSpan.innerText = temperature.value;
    // tempUnitSpan.innerText = temperature.unit;

    const root = document.documentElement;
    if(temperature.value <= 0)
    {
        root.style.setProperty('--temperature-icon-color', 'var(--cold)');
    }
    else if(temperature.value > 0 && temperature.value < 15)
    {
        root.style.setProperty('--temperature-icon-color', 'var(--cool)');
    }
    else if(temperature.value >= 15 && temperature.value <= 20)
    {
        root.style.setProperty('--temperature-icon-color', 'var(--normal)');
    }
    else if(temperature.value > 20 && temperature.value < 30)
    {
        root.style.setProperty('--temperature-icon-color', 'var(--warm)');
    }
    else if(temperature.value >= 30 && temperature.value <= 40)
    {
        root.style.setProperty('--temperature-icon-color', 'var(--hot)');
    }
    else
    {
        console.log('dead');
        root.style.setProperty('--temperature-icon-color', 'var(--dead)');
        document.querySelector('.card.temperature ion-icon').name = 'skull-outline';
    }

    humValueSpan.innerText = humidity.value;
    // humUnitSpan.innerText = humidity.unit;
}, 500);