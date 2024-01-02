/**
 * Jingga
 *
 * @package   Test
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#include <stdio.h>

#include "../../Utils/WebUtils.h"
#include "../../Utils/TestUtils.h"

int main(int argc, char** argv)
{
    printf("Utils:\n\n");
    printf("WebUtils:\n");
    Utils::WebUtils::setup();

    /* Single download */
    Utils::FileUtils::file_body single = Utils::WebUtils::download((char *) "https://jingga.app");

    ASSERT_CONTAINS(single.content, "Jingga");
    free(single.content);

    /* Multi download */
    /*
    const char *urls[] = {
        "https://jingga.app/terms",
        "https://jingga.app/imprint",
        "https://jingga.app/privacy"
    };

    const char *contains[] = {
        "Terms of Service",
        "Commercial register",
        "What is personal data?"
    };

    Utils::FileUtils::file_body *multi = Utils::WebUtils::multi_download((char **) urls, 3, 3);
    for (int i = 0; i < 3; ++i) {
        ASSERT_CONTAINS(multi[i].content, contains[i]);
        free(multi[i].content);
    }

    free(multi);
    */

    Utils::WebUtils::clean();
    printf("\n\n");

    return 0;
}
