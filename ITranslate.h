#ifndef ITRANSLATE_H
#define ITRANSLATE_H

#include <QString>
class ITranslate
{
public:
    typedef enum{
        HanToEnglis,
        EnglisToHan
    }E_Language;

    ITranslate();
    virtual QString StartTranslate(QString text, E_Language = EnglisToHan) = 0;
};

#endif // ITRANSLATE_H
