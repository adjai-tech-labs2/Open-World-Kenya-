#pragma once

#include <string>
#include <memory>
#include <vector>
#include "core/error_handling/result.h"

namespace kow {

class IWindow {
public:
    virtual ~IWindow() = default;
    virtual bool create(const std::string& title, int width, int height) = 0;
    virtual void destroy() = 0;
    virtual bool is_open() const = 0;
    virtual void set_title(const std::string& title) = 0;
    virtual void set_size(int width, int height) = 0;
    virtual void get_size(int& width, int& height) const = 0;
    virtual void set_position(int x, int y) = 0;
    virtual void show() = 0;
    virtual void hide() = 0;
    virtual void focus() = 0;
    virtual bool is_focused() const = 0;
    virtual void* native_handle() const = 0;
};

class WindowAdapter : public IWindow {
public:
    using CreateFn = std::function<bool(const std::string&, int, int)>;

    void set_on_create(CreateFn fn) { on_create_ = std::move(fn); }

    bool create(const std::string& title, int width, int height) override {
        if (on_create_) return on_create_(title, width, height);
        return false;
    }

    void destroy() override {}
    bool is_open() const override { return false; }
    void set_title(const std::string&) override {}
    void set_size(int, int) override {}
    void get_size(int& w, int& h) const override { w = 1280; h = 720; }
    void set_position(int, int) override {}
    void show() override {}
    void hide() override {}
    void focus() override {}
    bool is_focused() const override { return false; }
    void* native_handle() const override { return nullptr; }
};

class WindowListener {
public:
    virtual ~WindowListener() = default;
    virtual void on_window_resized(int width, int height) {}
    virtual void on_window_focus(bool focused) {}
    virtual void on_window_close() {}
};

class WindowManager {
public:
    static UserManager& instance();

    void create(const std::string& title, int width, int height);
    void destroy();
    void update();
    void set_title(const std::string& title);
    void set_size(int width, int height);
    void get_size(int& width, int& height) const;
    bool is_open() const;
    bool is_focused() const;
    void add_listener(std::unique_ptr<WindowListener> listener);

    IWindow* window() { return window_.get(); }
    const IWindow* window() const { return window_.get(); }
    void set_window(std::unique_ptr<IWindow> window) { window_ = std::move(window); }

private:
    UserManager() = default;
    std::unique_ptr<IWindow> window_;
    std::vector<std::unique_ptr<WindowListener>> listeners_;
};

} // namespace kow