#-------------------------------------------------
#
# Project created by QtCreator 2013-06-06T11:42:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = flowmaster
TEMPLATE = app


win32 {
		static {
			message("Static Build")
		}

		INCLUDEPATH += $$quote(D:\\Programming\\flowmaster\\libpumpcontrol)

		LIBS += flowmaster.lib
		LIBS += Advapi32.lib

		DEFINES += NO_ENUMSERIAL_USING_WMI
		DEFINES += CENUMERATESERIAL_USE_STL
		DEFINES += NO_ENUMSERIAL_USING_CREATEFILE
		DEFINES += NO_ENUMSERIAL_USING_QUERYDOSDEVICE
		DEFINES += NO_ENUMSERIAL_USING_GETDEFAULTCOMMCONFIG
		DEFINES += NO_ENUMSERIAL_USING_ENUMPORTS

		DEFINES += _UNICODE

		INCLUDEPATH += $$quote(D:\\Programming\\enumser)

		CONFIG(debug,debug|release){
			INCLUDEPATH += $$quote(D:\\Programming\\Qt\\Qwt-debug-6.1.2\\include)
			LIBS += /LIBPATH:$$quote(D:\\Programming\\Qt\\Qwt-debug-6.1.2\\lib)
			QMAKE_LFLAGS += /LIBPATH:$$quote(D:\\Programming\\flowmaster\\libpumpcontrol\\flowmaster_vcproj\\x64\\Debug)
			LIBS += qwtd.lib
		}

		CONFIG(release,debug|release){
			QMAKE_CXXFLAGS += /GL
			QMAKE_LFLAGS += /LTCG
			LIBS += qwt.lib

			static {
				INCLUDEPATH += $$quote(D:\\Programming\\Qt\\Qwt-release-static-6.1.2\\include)
				LIBS += /LIBPATH:$$quote(D:\\Programming\\Qt\\Qwt-release-static-6.1.2\\lib)
				QMAKE_LFLAGS += /LIBPATH:"$$quote(D:\\Programming\\flowmaster\\libpumpcontrol\\flowmaster_vcproj\\x64\\rel_static)"
				DEFINES += FM_STATIC
			}
			!static{
				INCLUDEPATH += $$quote(D:\\Programming\\Qt\\Qwt-release-6.1.2\\include)
				LIBS += /LIBPATH:$$quote(D:\\Programming\\Qt\\Qwt-release-6.1.2\\lib)
				QMAKE_LFLAGS += /LIBPATH:$$quote(D:\\Programming\\flowmaster\\libpumpcontrol\\flowmaster_vcproj\\x64\\Release)
			}
		}

		DEFINES += FM_WIN
		DEFINES += NVAPI

		INCLUDEPATH += $$quote(D:\\Programming\\libs\\nvidia)
		LIBS += /LIBPATH:$$quote(D:\\Programming\\libs\\nvidia\\amd64) nvapi64.lib

		INCLUDEPATH += $$quote(D:\\Programming\\boost_1_55_0)
}
unix {
		QMAKE_CXXFLAGS += -std=c++14
		QMAKE_CXXFLAGS_RELEASE += -flto
		QMAKE_LFLAGS_RELEASE += -flto
		INCLUDEPATH += $$quote(/usr/include/qwt)
		INCLUDEPATH += $$quote(/home/matt/src/flowstuff/libflowmaster)
		INCLUDEPATH += $$quote(/home/matt/src/tdk_5.319.43/nvml/include)

		LIBS += -ldl

		CONFIG(debug,debug|release){
				LIBS += -L$$quote(/usr/lib/debug/usr/lib64/)
				LIBS += -L$$quote(/home/matt/src/flowstuff/libflowmaster/)
				LIBS += -lqwt
				LIBS += -lflowmaster
		}

		CONFIG(release,debug|release){
				LIBS += -L$$quote(/home/matt/src/flowstuff/libflowmaster/)
				LIBS += -lqwt
				LIBS += -lflowmaster_static
		}

		DEFINES += FM_LINUX
}

SOURCES += main.cpp\
		mainwindow.cpp \
		mtthermo.cpp \
		tachometer.cpp \
		serialportdialog.cpp \
		logger.cpp \
		monitor.cpp \
		flowstate.cpp \
		flowmasterwidget.cpp \
		gpu_api.cpp \
		mtclickablelabel.cpp \

HEADERS  += mainwindow.hpp \
		mtthermo.hpp \
		tachometer.hpp \
		serialportdialog.hpp \
		defines.h \
		logger.hpp \
		monitor.hpp \
		flowstate.hpp \
		flowmasterwidget.hpp \
		gpu_api.hpp \
		mtclickablelabel.hpp \

win32 {
		SOURCES += \
		nvapi_wrapper.cpp \
		serialportdialog_win32.cpp\
		enumser.cpp

		HEADERS +=\
		nvapi_wrapper.hpp
}

unix {
		SOURCES += serialportdialog_linux.cpp\
			nvml_wrapper.cpp

		HEADERS += nvml_wrapper.hpp

}
