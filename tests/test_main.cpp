#include <QCoreApplication>
#include <QtTest>

#include "motion-det-app/mainwindow.h"
#include "motion-det-app/translations/motiondetapplanguagetranslator.h"

class TestMotionDetApp : public QObject {
    Q_OBJECT

public:
    TestMotionDetApp() = default;
    ~TestMotionDetApp() = default;

private slots:
    void testTranslationsExistence();
    void testStartup();
};

void TestMotionDetApp::testTranslationsExistence() {
    using LanguageTranslator = msd::MotionDetAppLanguageTranslator;
    const QStringList expectedLanguages = {
            "ru",
    };

    auto& instance = LanguageTranslator::instance();
    const auto languageToRestore = instance.language();
    for (const auto& expectedLanguage : expectedLanguages) {
        instance.setLanguage(expectedLanguage);
        QCOMPARE(instance.language(), expectedLanguage);
    }

    instance.setLanguage(languageToRestore);
}

void TestMotionDetApp::testStartup() {
    MainWindow w;
    w.show();

    QVERIFY(true);
}

QTEST_MAIN(TestMotionDetApp)

#include "test_main.moc"
