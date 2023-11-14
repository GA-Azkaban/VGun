#include <Windows.h>
#include "YunuAnimInstance.h"
#include "ResourceManager.h"


void YunuAnimInstance::SetAnimation(LPCSTR animID)
{
    SetAnimation(ResourceManager::Instance.Get().GetYunuAnimation(animID));
}
