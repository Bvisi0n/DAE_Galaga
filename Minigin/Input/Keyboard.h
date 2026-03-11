#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <memory>

namespace dae
{

    class Keyboard final
    {
    public:
        enum class Key { W, A, S, D };

        Keyboard();
        ~Keyboard();

        void Update();
        bool IsDown(Key key) const;
        bool IsUp(Key key) const;
        bool IsPressed(Key key) const;

    private:
        class KeyboardImpl;
        std::unique_ptr<KeyboardImpl> m_pImpl;
    };
}

#endif
