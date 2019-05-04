#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    ui->aboutText->setText(QString(
                "%1 version %2 by %3 %4.\n"
                "GitHub: %5\n"
                "License: MIT\n"
                "\n"
                "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, "
                "EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF "
                "MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. "
                "IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY "
                "CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, "
                "TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE "
                "SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE."
                )
            .arg(PROGRAM_NAME, PROGRAM_VERSION, PROGRAM_AUTHOR_FIRST_NAME, PROGRAM_AUTHOR_LAST_NAME, PROGRAM_REPO)
       );
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
