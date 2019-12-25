/********************************************************************
Copyright 2019 Roman Gilg <subdiff@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*********************************************************************/
#ifndef COMMON_CONTROL_H
#define COMMON_CONTROL_H

#include <kscreen/types.h>

#include <QObject>
#include <QVariantMap>

class Control : public QObject
{
    Q_OBJECT
public:
    enum class OutputRetention {
        Undefined = -1,
        Global = 0,
        Individual = 1,
    };
    Q_ENUM(OutputRetention)

    explicit Control(QObject *parent = nullptr);


    ~Control() override = default;

    bool writeFile();

protected:
    virtual QString dirPath() const;
    virtual QString filePath() const = 0;
    QString filePathFromHash(const QString &hash) const;
    void readFile();
    QVariantMap& info();
    const QVariantMap& constInfo() const;

    static OutputRetention convertVariantToOutputRetention(QVariant variant);

private:
    static QString s_dirName;
    QVariantMap m_info;
};

class ControlConfig : public Control
{
    Q_OBJECT
public:
    explicit ControlConfig(KScreen::ConfigPtr config, QObject *parent = nullptr);

    OutputRetention getOutputRetention(const KScreen::OutputPtr &output) const;
    OutputRetention getOutputRetention(const QString &outputId, const QString &outputName) const;
    void setOutputRetention(const KScreen::OutputPtr &output, OutputRetention value);
    void setOutputRetention(const QString &outputId, const QString &outputName, OutputRetention value);

    QString dirPath() const override;
    QString filePath() const override;

private:
    QVariantList getOutputs() const;
    void setOutputs(QVariantList outputsInfo);
    bool infoIsOutput(const QVariantMap &info, const QString &outputId, const QString &outputName) const;

    KScreen::ConfigPtr m_config;
    QStringList m_duplicateOutputIds;
};

class ControlOutput : public Control
{
    Q_OBJECT
public:
    explicit ControlOutput(KScreen::OutputPtr output, QObject *parent = nullptr);

    // TODO: scale auto value

    QString dirPath() const override;
    QString filePath() const override;

private:
    KScreen::OutputPtr m_output;
};

#endif
