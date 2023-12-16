#ifndef STATEMACHINE_H
#define STATEMACHINE_H


class stateMachine
{
    public:
        stateMachine();
        virtual ~stateMachine();

        void changeState();
        void writeError();
        void sendError();
        void writeTelem();
        void sendTelem();

    protected:

    private:
};

#endif // STATEMACHINE_H
