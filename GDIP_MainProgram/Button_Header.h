#define maxButtonCount 8  // maximum callback function count

typedef void (*OnClickButtonCallback)();

// Functions :
void SetUpOnUpCallback(int, OnClickButtonCallback);
void SetUpOnDownCallback(int, OnClickButtonCallback);
void SetUpOnHoldCallback(int, OnClickButtonCallback);
void SetUpOnNonCallback(int, OnClickButtonCallback);
void CheckButtonOnClick(int, int);

static OnClickButtonCallback onUpCallback[maxButtonCount];
static OnClickButtonCallback onDownCallback[maxButtonCount];
static OnClickButtonCallback onHoldCallback[maxButtonCount];
static OnClickButtonCallback onNonCallback[maxButtonCount];
static bool                  buttonDown[maxButtonCount];

void SetUpOnUpCallback(int buttonIndex, OnClickButtonCallback cb)
{
  if (buttonIndex >= maxButtonCount)
  {
    Serial.println("[Warning] Over the maximum of button callback count.");
    return;
  }
  buttonDown[buttonIndex]   = false;
  onUpCallback[buttonIndex] = cb;
  Serial.println("Set up OnUp Button Callback.");
}

void SetUpOnDownCallback(int buttonIndex, OnClickButtonCallback cb)
{
  if (buttonIndex >= maxButtonCount)
  {
    Serial.println("[Warning] Over the maximum of button callback count.");
    return;
  }
  buttonDown[buttonIndex]     = false;
  onDownCallback[buttonIndex] = cb;
  Serial.println("Set up OnDown Button Callback.");
}

void SetUpOnHoldCallback(int buttonIndex, OnClickButtonCallback cb)
{
  if (buttonIndex >= maxButtonCount)
  {
    Serial.println("[Warning] Over the maximum of button callback count.");
    return;
  }
  buttonDown[buttonIndex]     = false;
  onHoldCallback[buttonIndex] = cb;
  Serial.println("Set up OnHold Button Callback.");
}

void SetUpOnNonCallback(int buttonIndex, OnClickButtonCallback cb)
{
  if (buttonIndex >= maxButtonCount)
  {
    Serial.println("[Warning] Over the maximum of button callback count.");
    return;
  }
  buttonDown[buttonIndex]    = false;
  onNonCallback[buttonIndex] = cb;
  Serial.println("Set up OnNon Button Callback.");
}


void CheckButtonOnClick(int buttonIndex, int pin)
{
  int getOnClick = digitalRead(pin);
  
  if (getOnClick == 1)
  {
    if(!buttonDown[buttonIndex])
    {
      buttonDown[buttonIndex] = true;
      
      // printf("pin [%d] Button down (%d).\n", pin, getOnClick);
      if (onDownCallback[buttonIndex] != NULL)
        onDownCallback[buttonIndex]();
    }

    if (onHoldCallback[buttonIndex] != NULL)
      onHoldCallback[buttonIndex]();
  }
  else
  {
    if (onNonCallback[buttonIndex] != NULL)
      onNonCallback[buttonIndex]();

    if (buttonDown[buttonIndex])
    {
      buttonDown[buttonIndex] = false;
      
      // printf("pin [%d] Button up   (%d).\n", pin, getOnClick);
      if (onUpCallback[buttonIndex] != NULL)
        onUpCallback[buttonIndex]();
    }
  }
}
