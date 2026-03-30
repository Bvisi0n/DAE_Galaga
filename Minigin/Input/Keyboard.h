#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <memory>

namespace dae
{

    class Keyboard final
    {
    public:
        enum class Key
        {
            W, A, S, D,
            Space, Q, E
        };

        Keyboard();
        ~Keyboard();

        void Update();
        [[nodiscard]] bool IsDown(const Key key) const;
        [[nodiscard]] bool IsUp(const Key key) const;
        [[nodiscard]] bool IsPressed(const Key key) const;

    private:
        class KeyboardImpl;
        std::unique_ptr<KeyboardImpl> m_pImpl;
    };
}

#endif
