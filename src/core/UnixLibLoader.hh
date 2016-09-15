#pragma once

# include	<iostream>
# include	"ILibLoader.hh"

class UnixLibLoader : public ILibLoader
 {
 private:
   void		*m_openedLib;

public:
  UnixLibLoader();
  ~UnixLibLoader();

private:
  UnixLibLoader(const UnixLibLoader &);
  UnixLibLoader &operator=(const UnixLibLoader &);

protected:
  virtual void	open(std::string const &libPath) throw(ClassException);
  virtual void	*findSymbol(std::string const &symbol) throw(ClassException);

public:
  virtual void	close();
};

