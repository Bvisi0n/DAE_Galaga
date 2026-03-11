#if WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#else
    #include <SDL3/SDL.h>
#endif

#include <unordered_map>
#include <vector>

#include "Keyboard.h"

namespace dae
{
    class Keyboard::KeyboardImpl
    {
    public:
        void Update()
        {
            m_PreviousState = m_CurrentState;

            #if WIN32
                BYTE state[256];
                if (GetKeyboardState(state))
                {
                    m_CurrentState.assign(state, state + 256);
                }
            #else
                int numKeys;
                const bool* state = SDL_GetKeyboardState(&numKeys);
                m_CurrentState.assign(state, state + numKeys);
            #endif

            if (m_PreviousState.empty())
            {
                m_PreviousState = m_CurrentState;
            }
        }

        bool IsPressed(Key key) const
        {
            int platform_key = GetPlatformKey(key);
            #if WIN32
                return m_CurrentState[platform_key] & 0x80;
            #else
                return m_CurrentState[platform_key];
            #endif
        }

        bool IsDown(Key key) const
        {
            return IsPressed(key) && !WasPressed(key);
        }

        bool IsUp(Key key) const
        {
            return !IsPressed(key) && WasPressed(key);
        }

    private:
        bool WasPressed(Key key) const
        {
            int platform_key = GetPlatformKey(key);
            #if WIN32
                return m_PreviousState[platform_key] & 0x80;
            #else
                return m_PreviousState[platform_key];
            #endif
        }

        int GetPlatformKey(Key key) const
        {
            #if WIN32
                // https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
                static const std::unordered_map<Key, int> key_map =
                {
                    {Key::W, 'W'},
                    {Key::A, 'A'},
                    {Key::S, 'S'},
                    {Key::D, 'D'},
                };
            #else
                // https://wiki.libsdl.org/SDL3/SDL_Scancode
                static const std::unordered_map<KeyboardKey, int> key_map =
                {
                    {Key::W, SDL_SCANCODE_W},
                    {Key::A, SDL_SCANCODE_A},
                    {Key::S, SDL_SCANCODE_S},
                    {Key::D, SDL_SCANCODE_D},
                };
            #endif
            return key_map.at(key);
        }

        std::vector<unsigned char> m_CurrentState;
        std::vector<unsigned char> m_PreviousState;
    };

    Keyboard::Keyboard()
        : m_pImpl(std::make_unique<KeyboardImpl>()) {}

    Keyboard::~Keyboard() = default;

    void Keyboard::Update()
    {
        m_pImpl->Update();
    }

    bool Keyboard::IsDown(Key key) const
    {
        return m_pImpl->IsDown(key);
    }

    bool Keyboard::IsUp(Key key) const
    {
        return m_pImpl->IsUp(key);
    }

    bool Keyboard::IsPressed(Key key) const
    {
        return m_pImpl->IsPressed(key);
    }
}
