# Maintainer: Kyle Laker <kyle@laker.email>

pkgname=xorlist
pkgver=0.1.0
pkgrel=1
pkgdesc="Library for an xorlist"
arch=("x86_64")
url="https://github.com/kylelaker/xorlist"
license=(MIT)
depends=()
makedepends=()
source=("${url}/archive/v${pkgver}.tar.gz")
sha512sums=('SKIP')

build() {
  cd "$srcdir/$pkgname-${pkgver}"
  make
}

check() {
  cd "$srcdir/$pkgname-${pkgver}"
  make test
}

package() {
  cd "$srcdir/$pkgname-${pkgver}"
  make DESTDIR="$pkgdir" install
}
