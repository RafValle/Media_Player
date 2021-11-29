#include "lib_requests.h"
#include "lib_manager.h"

#include "network_manager.h"

namespace lib {

static const QByteArray grant_type = QByteArrayLiteral("client_credentials");
static const QByteArray client_id = QByteArrayLiteral("31ebd438dad749dbb2061eb2bf2fd08e");
static const QByteArray client_secret = QByteArrayLiteral("b129f5074e1a45a2b0c172b15515a8ee");

static const QByteArray header_content_type = QByteArrayLiteral("Content-Type");
static const QByteArray header_x_www_form_urlencoded = QByteArrayLiteral("application/x-www-form-urlencoded");
static const QByteArray header_json = QByteArrayLiteral("application/json");

static const QByteArray header_authorization = QByteArrayLiteral("Authorization");
static const QByteArray header_basic = QByteArrayLiteral("Basic ");
static const QByteArray form_grant_type = QByteArrayLiteral("grant_type=");
static const QByteArray basic_colon = QByteArrayLiteral(":");

static const QByteArray tag_access_token= QByteArrayLiteral("access_token");
static const QByteArray tag_expires_in  = QByteArrayLiteral("expires_in")  ;
static const QByteArray tag_token_type  = QByteArrayLiteral("token_type")  ;

static const QByteArray token_url = QByteArrayLiteral("https://accounts.spotify.com/api/token");
static const QByteArray base_url = QByteArrayLiteral("https://api.spotify.com/v1");

Requests &Requests::instance()
{
    static Requests __i;
    return __i;
}

QVariant Requests::search(const QString q, int offset)
{
    auto base = QStringLiteral("/search?q=").append(q);
    base.append("&type=track&market=BR&include_external=audio");
    base.append("&offset=").append(QString::number(offset));
    return this->baseRequest(base.toUtf8());
}

QVariant Requests::markets()
{
    //"markets"[]
    return this->baseRequest(QByteArrayLiteral("/markets"));
}

QVariant Requests::genreSeeds()
{
    //"genres"[]
    return this->baseRequest(QByteArrayLiteral("/recommendations/available-genre-seeds"));
}

QVariant Requests::categories()
{
    //QVariantMap filter
    return this->baseRequest(QByteArrayLiteral("/browse/categories"));
}

QVariant Requests::baseRequest(QByteArray route)
{
    QVariant RETURN;
    if (!checkTokenValid())
        return RETURN;

    network::Request request;
    request.headers.insert(header_content_type , header_json );
    request.headers.insert(header_authorization, _token );
    request.url.setUrl(base_url + route);

    if (network::Manager::instance().call(request)){
        QJsonDocument docResponse = QJsonDocument::fromJson(request.response.body);
        lib::Manager::instance().saveRequest( request.response.code,
                                              docResponse.toJson(QJsonDocument::Compact),
                                              request.url.toString().toUtf8() );
        return docResponse.toVariant();
    } else {
        qDebug() << request.response.body;
    }
    return RETURN;
}

bool Requests::isTokenValid(){
    if (!_validation.isValid())
        return false;
    if (_token.simplified().isEmpty())
        return false;
    int secsToNow =_validation.currentDateTime().secsTo(_validation);
    return (secsToNow>10);
}

bool Requests::checkTokenValid()
{
    if (!this->isTokenValid()){
        if (!this->getToken()){
            qDebug() << "erro ao buscar token de acesso";
            return false;
        }
    }

    if (!this->isTokenValid()){
        qDebug() << "token continua invalido";
        return false;
    }
    return true;
}

bool Requests::getToken()
{
    QByteArray basic = client_id+basic_colon+client_secret;

    network::Request request;
    request.headers.insert(header_content_type  , header_x_www_form_urlencoded );
    request.headers.insert(header_authorization , header_basic + basic.toBase64() );
    QString tokenTeste = basic.toBase64();
    request.body = form_grant_type+grant_type;
    request.url.setUrl(token_url);
    request.method = request.POST;

    if (network::Manager::instance().call(request)){
        _accessToken = QJsonDocument::fromJson(request.response.body).toVariant().toMap();
        if(_accessToken.isEmpty()){
            return false;
        } else {
            QByteArray token      = _accessToken.value(tag_access_token).toByteArray();
            QByteArray expires_in = _accessToken.value(tag_expires_in  ).toByteArray();
            QByteArray token_type = _accessToken.value(tag_token_type  ).toByteArray();

            _validation = QDateTime::currentDateTime().addSecs(expires_in.toLongLong());
            _token = token_type.append(' ').append(token);
            qDebug() << _token;
            return true;
        }
    }
    return false;
}

}
